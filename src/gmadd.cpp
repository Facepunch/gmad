
#include "gmadd.h"

#ifdef _WIN32
#include "Windows.h"
#endif

using namespace Bootil;

int main( int argc, char* argv[] )
{
	Debug::SuppressPopups( true );
	CommandLine::Set( argc, argv );
	Console::FGColorPush( Console::Green );
	Output::Msg( "Garry's Mod Addon Creator 1.1\n" );
	Console::FGColorPop();
	
	//
	// Detect if this is a standalone cmd window, or was run as a script in an existing window
	// If we are standalone, pause on error, so it doesn't look like nothing happened.
	//
	bool pauseOnError = false;
#ifdef _WIN32
	DWORD iConsoleProcess;
	GetWindowThreadProcessId( GetConsoleWindow(), &iConsoleProcess );
	if ( GetCurrentProcessId() == iConsoleProcess ) pauseOnError = true;
#endif

	//
	// Get the command from the command line - (it should be argument 0)
	//
	BString strCommand = String::GetLower( CommandLine::GetArg( 0 ) );
	//Output::Msg( strCommand.c_str() );

	//
	// Create
	//
	if ( strCommand == "create" || File::IsFolder( strCommand ) )
	{

		BString strFolder = CommandLine::GetSwitch( "-folder", "" );
		if ( strFolder == "" && strCommand != "create" )
		{
			strFolder = strCommand;
		}

		if ( strFolder == "" )
		{
			Output::Msg( "Missing -folder (the folder to turn into an addon)\n" );
			exit( 1 );
		}

		BString strTarget = CommandLine::GetSwitch( "-out", "" );

		bool WarnOnInvalidFiles = CommandLine::GetFull().find( "-warninvalid" ) != -1;

		return CreateAddonFile( strFolder, strTarget, WarnOnInvalidFiles, pauseOnError );
	}

	//
	// Extract
	//
	if ( strCommand == "extract" || String::File::GetFileExtension( strCommand ) == "gma" )
	{
		BString strFile = CommandLine::GetSwitch( "-file", "" );
		bool pauseOnError = false;
		if ( strFile == "" && strCommand != "extract" )
		{
			strFile = strCommand;
		}

		if ( strFile == "" )
		{
			Output::Msg( "Missing -file (the addon you want to extract)\n" );
			exit( 1 );
		}

		BString strTarget = CommandLine::GetSwitch( "-out", "" );
		return ExtractAddonFile( strFile, strTarget, pauseOnError );
	}

	//
	// Help
	//
	Output::Msg("\nUsage:\n\n");

	Output::Msg("\tDrag'n'drop a .gma onto the gmad.exe to extract it\n");
	Output::Msg("\tDrag'n'drop a folder onto the gmad.exe to convert it to a .gma\n\n");

	Output::Msg("\tgmad.exe create -folder path/to/folder -out path/to/gma.gma\n");
	Output::Msg("\tgmad.exe create -folder path/to/folder\n");
	Output::Msg("\tgmad.exe extract -file path/to/gma.gma -out path/to/folder\n");
	Output::Msg("\tgmad.exe extract -file path/to/gma.gma\n\n");

	Output::Msg("\tAdd -warninvalid to automatically skip invalid files\n\n");
	Output::Msg("\tAdd -quiet to not spam file paths to output\n\n");

	// Make sure they see how to use it
	if ( pauseOnError ) Pause();

	return 0;
}
