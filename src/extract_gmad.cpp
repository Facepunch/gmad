
#include "gmadd.h"
#include "AddonWhiteList.h"
#include "AddonFormat.h"
#include "AddonReader.h"

using namespace Bootil;


int ExtractAddonFile( Bootil::BString strFile, Bootil::BString strOutPath )
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
			File::Write( strOutPath + entry->strName, filecontents );
		}
		else
		{
			Output::Warning( "\t\tCouldn't extract!" );
		}
	}

	Output::Msg( "Done!\n" );
	
	return 0;
}