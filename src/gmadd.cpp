
#include "gmadd.h"

using namespace Bootil;

int main( int argc, char *argv[] )
{
	CommandLine::Set( argc, argv );

	Output::Msg( "Garry's Addon Creator 1.0\n" );

	//
	// Get the command from the command line - (it should be argument 0)
	//
	BString strCommand = String::GetLower( CommandLine::GetArg( 0 ) );

	if ( strCommand == "create" )
	{
		Bootil::BString strFolder = Bootil::CommandLine::GetSwitch( "-folder", "" );
		if ( strFolder == "" )
		{
			Bootil::Output::Msg( "Missing -folder (the folder to turn into an addon)\n" );
			exit( 1 );
		}

		Bootil::BString strTarget = Bootil::CommandLine::GetSwitch( "-out", "" );
		if ( strTarget == "" )
		{
			Bootil::Output::Msg( "Missing -out (the filename of the target gma)\n" );
			exit( 1 );
		}

		return CreateAddonFile( strFolder, strTarget );

	}

	//
	// Help
	//
	Bootil::Output::Msg( "Usage:\n\n" );

	return 0;
}