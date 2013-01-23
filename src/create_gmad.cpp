
#include "gmadd.h"
#include "AddonWhiteList.h"
#include "AddonFormat.h"

using namespace Bootil;

namespace CreateAddon
{
	bool VerifyFiles( Bootil::String::List& files )
	{
		bool bOk = true;

		//
		// Bail out if there's no files
		//
		if ( files.empty() ) 
		{ 
			Output::Warning( "No files found, can't continue!\n" ); 
			bOk = false; 
		}

		
		//
		// Print each found file, check they're ok
		//
		BOOTIL_FOREACH( file, files, Bootil::String::List )
		{
			Output::Msg( "\t%s\n", file->c_str() );

			//
			// Check the file against the whitelist
			// Lowercase the name (addon filesystem is case insensitive)
			//
			if ( !Addon::WhiteList::Check( String::GetLower( *file ) ) )
			{
				Output::Warning( "\t\t[Not allowed by whitelist]\n" );
				bOk = false;
			}

			//
			// Warn that we're gonna lowercase the filename
			//
			if ( String::GetLower( *file )  != *file )
			{
				Output::Warning( "\t\t[Filename contains captial letters]\n" );
			}

		}

		return bOk;
	}

	//
	// Create an uncompressed GMAD file from a list of files
	//
	bool Create( Bootil::Buffer& buffer, Bootil::BString strFolder, Bootil::String::List& files )
	{
		// Header (5)
		buffer.Write( Addon::Ident, 4 );				// Ident (4)
		buffer.WriteType( (char) Addon::Version );		// Version (1)

		// SteamID (8) [unused]
		buffer.WriteType( (unsigned long long) 0ULL );

		// TimeStamp (8)
		buffer.WriteType( (unsigned long long) Bootil::Time::UnixTimestamp() );

		// Required content (a list of strings)
		buffer.WriteType( (char) 0 ); // signifies nothing

		// Addon Name (n) [unused]
		buffer.WriteString( "Untitled Addon" );

		// Addon Author (n) [unused]
		buffer.WriteString( "Author Name" );

		// Addon Version (4) [unused]
		buffer.WriteType( (int) 1 );

		// File list
		unsigned int iFileNum = 0;
		BOOTIL_FOREACH( f, files, String::List )
		{
			unsigned long	iCRC = Bootil::File::CRC( strFolder + *f );
			long long		iSize = Bootil::File::Size( strFolder + *f );

			iFileNum++;
			buffer.WriteType( (unsigned int) iFileNum );					// File number (4)
			buffer.WriteString( String::GetLower( *f ) );					// File name (all lower case!) (n)
			buffer.WriteType( (unsigned int) iCRC );						// File CRC (4)
			buffer.WriteType( (long long) iSize );							// File size (8)

			Output::Msg( "File index: %s [CRC:%u] [Size:%s]\n", f->c_str(), iCRC, String::Format::Memory( iSize ).c_str() );
		}

		// Zero to signify end of files
		iFileNum = 0;
		buffer.WriteType( (unsigned int) iFileNum );

		// The files
		BOOTIL_FOREACH( f, files, String::List )
		{
			Output::Msg( "Adding %s\n", f->c_str() );

			AutoBuffer filebuffer;
			Bootil::File::Read( strFolder + *f, filebuffer );
			if ( filebuffer.GetWritten() == 0 )
			{
				Output::Warning( "File %s seems to be empty (or we couldn't read it)\n", (strFolder + *f).c_str() );
				return 1;
			}

			buffer.WriteBuffer( filebuffer );
		}

		// CRC what we've written (to verify that the download isn't shitted) (4)
		unsigned long AddonCRC = Bootil::Hasher::CRC32::Easy( buffer.GetBase(), buffer.GetWritten() );
		buffer.WriteType( AddonCRC );

		return true;
	}
}


int CreateAddonFile( Bootil::BString strFolder, Bootil::BString strOutfile )
{
	bool bErrors = false;

	//
	// Make sure there's a slash on the end
	//
	String::File::FixSlashes( strFolder, "\\", "/" );
	String::Util::TrimRight( strFolder, "/" );
	strFolder = strFolder + "/";

	Output::Msg( "Looking in folder \"%s\"\n", strFolder.c_str() );

	//
	// Get a list of files in the specified folder
	//
	Bootil::String::List files;
	Bootil::File::GetFilesInFolder( strFolder, files, true );

	//
	// Sort the list into alphabetical order, no real reason - we're just ODC
	//
	Bootil::String::SortList( files, false );

	//
	// Verify
	//
	if ( !CreateAddon::VerifyFiles( files ) )
	{
		Output::Warning( "File list verification failed\n" );
		return 1;
	}

	//
	// Create an addon file in a buffer
	//
	Bootil::AutoBuffer buffer;
	if ( !CreateAddon::Create( buffer, strFolder, files ) )
	{
		Output::Warning( "Failed to create the addon\n" );
		return 1;
	}

	//
	// Save the buffer to the provided name
	//
	if ( !File::Write( strOutfile, buffer ) )
	{
		Output::Warning( "Couldn't save to file \"%s\"\n", strOutfile.c_str() );
		return 1;
	}

	//
	// Success!
	//
	Output::Msg( "Successfully saved to \"%s\" [%s]\n", strOutfile.c_str(), Bootil::String::Format::Memory( buffer.GetWritten() ).c_str() );

	return 0;
}