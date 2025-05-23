
#include "gmadd.h"
#include "AddonWhiteList.h"
#include "AddonFormat.h"
#include "Addon_Json.h"
#include <unordered_map>

using namespace Bootil;

namespace CreateAddon
{
	bool VerifyFiles( String::List& files, bool warnInvalid )
	{
		bool quiet = CommandLine::HasSwitch( "-quiet" );

		bool bOk = true;
		std::unordered_map<std::string, std::string> errorFiles;

		//
		// Bail out if there's no files
		//
		if ( files.empty() )
		{
			Output::Warning( "No files found, can't continue!\n" );
			bOk = false;
		}

		String::List old_files = files;
		files.clear();

		//
		// Print each found file, check they're ok
		//
		BOOTIL_FOREACH( file, old_files, String::List )
		{
			if ( !quiet ) Output::Msg( "\t%s\n", file->c_str() );

			//
			// Check the file against the whitelist
			// Lowercase the name (addon filesystem is case insensitive)
			//
			if ( Addon::WhiteList::Check( String::GetLower( *file ) ) )
			{
				files.push_back( *file );
			}
			else
			{
				if ( !quiet ) Output::Warning( "\t\t[Not allowed by whitelist]\n" );
				if ( !warnInvalid ) bOk = false;

				errorFiles.emplace( *file, "Not allowed by whitelist" );
			}	

			//
			// Warn that we're gonna lowercase the filename
			//
			if ( String::GetLower( *file ) != *file )
			{
				if ( !quiet ) Output::Warning( "\t\t[Filename contains captial letters]\n" );
			}
		}
		
		if ( !bOk )
		{
			Output::Msg( "The following files did not pass verification:\n" );
			
			for ( auto& it: errorFiles )
			{
				Output::Warning( "\t%s - %s\n", it.first.c_str(), it.second.c_str() );
			}
		}

		return bOk;
	}

	//
	// Create an uncompressed GMAD file from a list of files
	//
	bool Create( AutoBuffer& buffer, BString strFolder, String::List& files, BString strTitle, BString strDescription )
	{
		bool quiet = CommandLine::HasSwitch( "-quiet" );

		// CRCs are unused by the game
		bool doCRCs = true;
		if ( CommandLine::HasSwitch( "-nocrc" ) ) doCRCs = false;

		// Header (5)
		buffer.Write( Addon::Ident, 4 );				// Ident (4)
		buffer.WriteType( ( char ) Addon::Version );	// Version (1)
		// SteamID (8) [unused]
		buffer.WriteType( ( uint64_t ) 0ULL );
		// TimeStamp (8)
		buffer.WriteType( ( uint64_t ) Time::UnixTimestamp() );
		// Required content (a list of strings)
		buffer.WriteType( ( char ) 0 ); // signifies nothing
		// Addon Name (n)
		buffer.WriteString( strTitle );
		// Addon Description (n)
		buffer.WriteString( strDescription );
		// Addon Author (n) [unused]
		buffer.WriteString( "Author Name" );
		// Addon Version (4) [unused]
		buffer.WriteType( ( int32_t ) 1 );

		Output::Msg( "Writing file list...\n" );

		uint64_t iFileListSize = 0;
		BOOTIL_FOREACH( f, files, String::List )
		{
			iFileListSize = iFileListSize + 4 + 180 + 8 + 4; // File number (4) + File name (180 file name length limit) + File size (8) + File CRC (4)
		}

		if ( !buffer.EnsureCapacity( buffer.GetWritten() + iFileListSize ) )
		{
			Output::Warning( "Failed to allocate buffer. Expect problems!\n" );
		}

		// File list
		uint32_t iFileNum = 0;
		uint64_t iTotalSize = 0;
		BOOTIL_FOREACH( f, files, String::List )
		{
			int64_t	iSize = File::Size( strFolder + *f );
			if ( iSize <= 0 )
			{
				Output::Warning( "File '%s' seems to be empty, or we couldn't get its size! (errno=%i)\n", ( strFolder + *f ).c_str(), errno );
				return false;
			}

			iFileNum++;
			iTotalSize = iTotalSize + iSize;
			buffer.WriteType( ( uint32_t ) iFileNum );			// File number (4)
			buffer.WriteString( String::GetLower( *f ) );		// File name (all lower case!) (n)
			buffer.WriteType( ( int64_t ) iSize );				// File size (8)

			if ( doCRCs )
			{
				uint32_t iCRC = File::CRC( strFolder + *f );
				buffer.WriteType( ( uint32_t ) iCRC );			// File CRC (4)
			}
			else
			{
				buffer.WriteType( ( uint32_t ) 0 );
			}

			//Output::Msg( "\tFile index: %s [CRC:%u] [Size:%s]\n", f->c_str(), iCRC, String::Format::Memory( iSize ).c_str() );
		}

		if ( !buffer.EnsureCapacity( buffer.GetWritten() + iTotalSize + 8 ) )
		{
			Output::Warning( "Failed to allocate buffer. Expect problems!\n" );
		}

		// Zero to signify end of files
		iFileNum = 0;
		buffer.WriteType( ( uint32_t ) iFileNum );

		Output::Msg( "Writing files...\n" );

		// The files
		BOOTIL_FOREACH( f, files, String::List )
		{
			if ( !quiet ) Output::Msg( "\tWriting %s...\n", f->c_str() );

			AutoBuffer filebuffer;
			File::Read( strFolder + *f, filebuffer );

			if ( filebuffer.GetWritten() == 0 )
			{
				Output::Warning( "File '%s' seems to be empty (or we couldn't read it)\n", ( strFolder + *f ).c_str() );
				return false;
			}

			uint64_t before = buffer.GetWritten();
			buffer.WriteBuffer( filebuffer );
			uint64_t diff = buffer.GetWritten() - before;
			if ( diff < 1 )
			{
				Output::Warning( "Failed to write file '%s' - written %llu bytes! (Can't grow buffer?)\n", ( *f ).c_str(), diff );
				return false;
			}
		}

		// CRC what we've written (to verify that the download isn't shitted) (4)
		if ( doCRCs )
		{
			uint32_t AddonCRC = Hasher::CRC32::Easy( buffer.GetBase(), buffer.GetWritten() );
			buffer.WriteType<uint32_t>( AddonCRC );
		}
		else
		{
			buffer.WriteType<uint32_t>( 0 );
		}

		return true;
	}
}

int CreateAddonFile( BString strFolder, BString strOutfile, bool warnInvalid, bool pauseOnError )
{
	bool quiet = CommandLine::HasSwitch( "-quiet" );

	//
	// Make sure there's a slash on the end
	//
	String::File::FixSlashes( strFolder, "\\", "/" );
	String::Util::TrimRight( strFolder, "/" );
	strFolder = strFolder + "/";

	//
	// Make sure OutFile ends in .gma
	//
	if ( strOutfile.empty() )
	{
		strOutfile = strFolder;
		String::Util::TrimRight( strOutfile, "/" );
	}
	String::File::StripExtension( strOutfile );
	strOutfile += ".gma";
	Output::Msg( "Looking in folder \"%s\"\n", strFolder.c_str() );

	//
	// Load the Addon Info file
	//
	CAddonJson addoninfo( strFolder + "addon.json" );
	if ( !addoninfo.GetError().empty() )
	{
		Output::Warning( "%s error: %s\n", ( strFolder + "addon.json" ).c_str(), addoninfo.GetError().c_str() );
		if ( pauseOnError ) Pause();
		return 1;
	}

	//
	// Get a list of files in the specified folder
	//
	String::List files;
	File::GetFilesInFolder( strFolder, files, true );

	//
	// Let the addon json remove the ignored files
	//
	Output::Msg( "Removing ignored files...\n" );
	addoninfo.RemoveIgnoredFiles( files, quiet );

	//
	// Sort the list into alphabetical order, no real reason - we're just ODC
	//
	String::SortList( files, false );

	//
	// Verify
	//
	Output::Msg( "Verifying file list...\n" );
	if ( !CreateAddon::VerifyFiles( files, warnInvalid ) )
	{
		Output::Warning( "File list verification failed\n" );
		if ( pauseOnError ) Pause();
		return 1;
	}

	//
	// Create an addon file in a buffer
	//
	AutoBuffer buffer( 256 );
	if ( !CreateAddon::Create( buffer, strFolder, files, addoninfo.GetTitle(), addoninfo.BuildDescription() ) )
	{
		Output::Warning( "Failed to create the addon\n" );
		if ( pauseOnError ) Pause();
		return 1;
	}
	
	Output::Msg( "Writing the .gma...\n" );

	//
	// Save the buffer to the provided name
	//
	if ( !File::Write( strOutfile, buffer ) )
	{
		Output::Warning( "Couldn't save to file \"%s\"\n", strOutfile.c_str() );
		if ( pauseOnError ) Pause();
		return 1;
	}

	//
	// Success!
	//
	Output::Msg( "Successfully saved to \"%s\" [%s]\n", strOutfile.c_str(), String::Format::Memory( buffer.GetWritten() ).c_str() );
	Output::Msg( "Files ignored: %i\n", addoninfo.m_IgnoredFiles );
	return 0;
}