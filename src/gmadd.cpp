
#include "gmadd.h"

using namespace Bootil;

int main( int argc, char* argv[] )
{
	Debug::SuppressPopups( true );
	CommandLine::Set( argc, argv );
	Console::FGColorPush( Console::Green );
	Output::Msg( "Garry's Addon Creator 1.0\n" );
	Console::FGColorPop();
	//
	// Get the command from the command line - (it should be argument 0)
	//
	BString strCommand = String::GetLower( CommandLine::GetArg( 0 ) );

	//
	// Create
	//
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

		bool WarnOnInvalidFiles = Bootil::CommandLine::GetFull().find("-warninvalid") != -1;

		return CreateAddonFile( strFolder, strTarget, WarnOnInvalidFiles );
	}

	//
	// Extract
	//
	if ( strCommand == "extract" )
	{
		Bootil::BString strFile = Bootil::CommandLine::GetSwitch( "-file", "" );

		if ( strFile == "" )
		{
			Bootil::Output::Msg( "Missing -file (the addon you want to extract)\n" );
			exit( 1 );
		}

		Bootil::BString strTarget = Bootil::CommandLine::GetSwitch( "-out", "" );
		return ExtractAddonFile( strFile, strTarget );
	}

	//
	// Help
	//
	Bootil::Output::Msg( "Usage: gmad <action> [options]\n" );

	Bootil::Output::Msg( "  Actions:\n" );
	Bootil::Output::Msg( "    create                - Create a new .gma file\n" );
	Bootil::Output::Msg( "      Options:\n" );
	Bootil::Output::Msg( "        -folder <folder>  - Specifies the folder in which the addon resides\n" );
	Bootil::Output::Msg( "        -out <file>       - Specifies the .gma file\n" );
	Bootil::Output::Msg( "        -warninvalid      - Don't stop on disallowed files, only warn\n\n" );
	Bootil::Output::Msg( "    extract               - Extract an existing .gma file\n" );
	Bootil::Output::Msg( "      Options:\n" );
	Bootil::Output::Msg( "        -file <file>      - Specifies the .gma file which is to be extracted\n" );
	Bootil::Output::Msg( "        -out <folder>     - Specifies the destination folder\n\n" );

	Bootil::Output::Msg( "  Examples:\n" );
	Bootil::Output::Msg( "  Create a new .gma file:\n" );
	Console::FGColorPush( Console::White );
	Bootil::Output::Msg( "gmad create -folder C:\\path\\to\\addon\\folder\\ -out C:\\where\\to\\save\\file\\out.gma\n" );
	Console::FGColorPop();
	Bootil::Output::Msg( "  Extract an existing .gma file into its parent folder:\n" );
	Console::FGColorPush( Console::White );
	Bootil::Output::Msg( "gmad extract -file C:\\steam\\etc\\garrysmod\\addons\\my_addon_12345.gma\n" );
	Console::FGColorPop();
	Bootil::Output::Msg( "  Extract an existing .gma file into another folder:\n" );
	Console::FGColorPush( Console::White );
	Bootil::Output::Msg( "gmad extract -file C:\\etc\\garrysmod\\addons\\my_addon_12345.gma -out C:\\folder\n" );
	Console::FGColorPop();
	return 0;
}
