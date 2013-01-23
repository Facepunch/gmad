
#include "gmadd.h"

void AddFilesRecursive( Bootil::BString strSearch, Bootil::String::Map& list, int iBaseLen, Bootil::BString strBind )
{
	Bootil::BString strBaseDir = Bootil::String::File::GetStripFilename( strSearch );

	Bootil::String::File::FixSlashes( strSearch );

	Bootil::Output::Msg( "SEARCH: %s\n", strSearch.c_str() );

	Bootil::String::List files, folders;

	Bootil::File::Find( &files, &folders, strSearch, false );

	BOOTIL_FOREACH_CONST( f, files, Bootil::String::List )
	{
		Bootil::Output::Msg( "FILE: %s\n", f->c_str() );
		Bootil::BString strFileName = strBaseDir + *f;
		list[ strBind + strFileName.substr( iBaseLen ) ] = strFileName;
	}

	//
	// ** == recursive
	//
	if ( Bootil::String::File::GetExtractFilename( strSearch ) == "**" )
	{
		BOOTIL_FOREACH_CONST( f, folders, Bootil::String::List )
		{
			Bootil::Output::Msg( "FOLDER: %s\n", f->c_str() );
			AddFilesRecursive( Bootil::String::File::GetStripFilename( strSearch ) + *f + "\\**", list, iBaseLen, strBind );
		}
	}
	
}

void DoConfig( const Bootil::BString& strName )
{
	Bootil::Output::Msg( "Config: %s\n", strName.c_str() );

	Bootil::BString strFileContents;
	Bootil::BString strBasePath = Bootil::String::File::GetStripFilename( strName );

	Bootil::Output::Msg( "Base Path: %s\n", strBasePath.c_str() );

	//
	// Read the file, error if it wasn't found.
	//
	if ( !Bootil::File::Read( strName, strFileContents ) )
	{
		Bootil::Output::Msg( "Config file wasn't found!\n" );
		exit( 1 );
	}

	//
	// Parse the file, error out if we can't
	//
	Bootil::Data::Tree	tree;

	if ( !Bootil::Data::Json::Import( tree, strFileContents ) )
	{
		Bootil::Output::Msg( "Error parsing config file - is the json valid?\n" );
		exit( 1 );
	}

	Bootil::BString strConfigErrors = "";

	Bootil::BString strTitle = tree.ChildValue( "title", "" );
	if ( strTitle == "" ) strConfigErrors += "'title' is empty\n";

	Bootil::Output::Msg( "Addon Title: %s\n", strTitle.c_str() );

	Bootil::Data::Tree& contents = tree.GetChild( "contents" );
	if ( !contents.HasChildren() ) strConfigErrors += "'contents' is empty\n";

	if ( !strConfigErrors.empty() )
	{
		Bootil::Output::Msg( "The following config file errors were found:\n%s", strConfigErrors.c_str() );
		exit( 1 );
	}

	contents.Children();

	Bootil::String::Map FileList;

	BOOTIL_FOREACH_CONST( path, contents.Children(), Bootil::Data::Tree::List )
	{
		Bootil::BString strSearchPath = path->ChildValue( "path" );

		Bootil::Output::Msg( "Searching Path: %s%s\n", strBasePath.c_str(), strSearchPath.c_str() );
		AddFilesRecursive( strBasePath + strSearchPath, FileList, Bootil::String::File::GetStripFilename( strBasePath + strSearchPath ).length(), path->ChildValue( "bind" ) );
	}

	Bootil::Output::Msg( "Files\n" );
	BOOTIL_FOREACH_CONST( f, FileList, Bootil::String::Map )
	{
		Bootil::Output::Msg( "%s\t: %s\n", f->first.c_str(), f->second.c_str() );
	}
		
}

int main( int argc, char *argv[] )
{
	Bootil::CommandLine::Set( argc, argv );

	Bootil::Output::Msg( "Garry's Addon Creator 1.0\n" );

	//
	//
	//
	Bootil::BString strFolder = Bootil::CommandLine::GetSwitch( "-folder", "" );
	if ( strFolder != "" )
	{
		Bootil::BString strTarget = Bootil::CommandLine::GetSwitch( "-out", "" );
		if ( strTarget == "" )
		{
			Bootil::Output::Msg( "Missing -out\n" );
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