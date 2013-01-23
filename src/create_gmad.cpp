
#include "gmadd.h"
#include "AddonWhiteList.h"

using namespace Bootil;


int CreateAddonFile( Bootil::BString strFolder, Bootil::BString strOutfile )
{
	bool bErrors = false;

	Output::Msg( "Looking in folder \"%s\"\n", strFolder.c_str() );

	//
	// Get a list of files in the speified folder
	//
	Bootil::String::List files;
	Bootil::File::GetFilesInFolder( strFolder, files, true );

	//
	// Sort the list into alphabetical order, no real reason - we're just ODC
	//
	Bootil::String::SortList( files, false );

	//
	// Bail out if there's no files
	//
	if ( files.empty() ) { Output::Msg( "No files found, can't continue!\n" ); return 1; }

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
			Output::Warning( "\t\tNot allowed!\n" );
			bErrors = true;
		}

	}

	//
	// Bail if errors.
	//
	if ( bErrors ) { Output::Msg( "Errors detected.\n" ); return 1; }

	return 0;
}