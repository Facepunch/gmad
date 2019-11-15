
#include "gmadd.h"
#include "AddonWhiteList.h"
#include "AddonFormat.h"
#include "AddonReader.h"

using namespace Bootil;

int ExtractAddonFile( BString strFile, BString strOutPath )
{
	Output::Msg( "Opening \"%s\"\n", strFile.c_str() );

	//
	// If an out path hasn't been provided, make our own
	//
	if ( strOutPath.empty() )
	{
		strOutPath = String::File::GetStripExtension( strFile );
	}

	//
	// Remove slash, add slash (enforces a slash)
	//
	String::File::FixSlashes( strOutPath );
	String::Util::TrimRight( strOutPath, "/" );
	strOutPath = strOutPath + "/";

	Addon::Reader addon;
	if ( !addon.ReadFromFile( strFile ) )
	{
		Output::Warning( "There was a problem opening the file\n" );
		return 1;
	}

	if ( !addon.Parse() )
	{
		Output::Warning( "There was a problem parsing the file\n" );
		return 1;
	}

	Output::Msg( "Extracting Files:\n" );
	uint32_t badFileCount = 0;
	BOOTIL_FOREACH_CONST( entry, addon.GetList(), Addon::FileEntry::List )
	{
		Output::Msg( "\t%s [%s]\n", entry->strName.c_str(), String::Format::Memory( entry->iSize ).c_str() );
		
		// Make sure folders exists
		File::CreateFolder( strOutPath + String::File::GetStripFilename( entry->strName ), true );
		
		// Load the file into the buffer
		AutoBuffer filecontents;
		if ( addon.ReadFile( entry->iFileNumber, filecontents ) )
		{
			// Write the file to disk
			if ( !File::Write( strOutPath + entry->strName, filecontents ) )
			{
				BString genPath = "badnames/" + String::ToString( badFileCount ) + ".unk";
				Output::Warning( "\t\tCouldn't write, trying to write as '%s'..\n", genPath.c_str() );

				// Try to write the file but don't use any of its name, since we don't know which part of it may have caused the problem
				File::CreateFolder( strOutPath + "badnames/", true );
				File::Write( strOutPath + genPath, filecontents );
				badFileCount++;
			}
		}
		else
		{
			Output::Warning( "\t\tCouldn't extract!\n" );
		}
	}
	Output::Msg( "Done!\n" );
	return 0;
}
