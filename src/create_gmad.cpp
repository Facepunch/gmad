
#include "gmadd.h"

using namespace Bootil;


int CreateAddonFile( Bootil::BString strFolder, Bootil::BString strOutfile )
{
	Output::Msg( "Looking in folder \"%s\"\n", strFolder.c_str() );

	Bootil::String::List files;
	Bootil::File::GetFilesInFolder( strFolder, files, true );

	BOOTIL_FOREACH( file, files, Bootil::String::List )
	{
		Output::Msg( "\t%s\n", file->c_str() );
	}

	return 0;
}