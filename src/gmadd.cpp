
#include "gmadd.h"

using namespace Bootil;

int main( int argc, char* argv[] )
{
	Debug::SuppressPopups( true );
	CommandLine::Set( argc, argv );
	Console::FGColorPush( Console::Green );
	Output::Msg( "Garry's Mod Addon Creator 1.1\n" );
	Console::FGColorPop();

	//
	// Get the command from the command line - (it should be argument 0)
	//
	BString strCommand = String::GetLower( CommandLine::GetArg( 0 ) );
	//Output::Msg( strCommand.c_str() );

	//
	// Create
	//
	if ( strCommand == "create" || File::IsFolder( strCommand ) && strCommand != "extractFolder")
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

		bool WarnOnInvalidFiles = CommandLine::GetFull().find("-warninvalid") != -1;

		return CreateAddonFile( strFolder, strTarget, WarnOnInvalidFiles );
	}

	//
	// Extract
	//
	if ( strCommand == "extract" || String::File::GetFileExtension( strCommand ) == "gma" )
	{
		BString strFile = CommandLine::GetSwitch( "-file", "" );

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
		return ExtractAddonFile( strFile, strTarget );
	}
	
	//
	// Extract Folders
	//
	if ( strCommand == "extractfolder" )
	{
		BString strFolder = CommandLine::GetSwitch( "-folder", "" );
		
		if ( strFolder == "" )
		{
			Output::Msg( "Missing -folder (the folder you want to extract)\n" );
			exit( 1 );
		}
		
		BString strTarget = CommandLine::GetSwitch( "-out", "" );
		
		//
		// Get files to extract in folder
		//
		String::List files;
		File::GetFilesInFolder( strFolder, files, false );
		
		BOOTIL_FOREACH( f, files, String::List )
		{
			BString filenameWithExtension = f->c_str();
			BString filename = filenameWithExtension;
			String::File::StripExtension(filename);
			if (String::File::IsFileExtension(filenameWithExtension, "gma"))
			{
				ExtractAddonFile(strFolder + "/" + filenameWithExtension, strTarget + "/" + filename);
			}
		}
		return 0;
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
	Output::Msg("\tgmad.exe extract -file path/to/gma.gma\n");
	Output::Msg("\tgmad.exe extractFolder -folder path/to/gma/files/ -out path/to/folder\n\n");

	Output::Msg("\tAdd -warninvalid to automatically skip invalid files\n\n");

#ifdef _WIN32
	// Make sure they see how to use it
	// Linux folks are too smart for this
	system( "pause" );
#endif

	return 0;
}
