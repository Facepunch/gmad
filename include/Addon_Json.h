
#ifndef ADDON_JSON_H
#define ADDON_JSON_H

#include "Addon_Util.h"

class CAddonJson
{
	public:

		CAddonJson( Bootil::BString strInfoFile )
		{
			m_IgnoredFiles = 0;

			Bootil::BString strFileContents;

			//
			// Try to open the file
			//
			if ( !Bootil::File::Read( strInfoFile, strFileContents ) )
			{
				m_strError = "Couldn't find file";
				return;
			}

			Bootil::Data::Tree tree;

			//
			// Parse the JSON
			//
			if ( !Bootil::Data::Json::Import( tree, strFileContents.c_str() ) )
			{
				m_strError = "Couldn't parse json";
				return;
			}

			//
			// Check the title
			//
			m_Title		= tree.ChildValue( "title" );

			if ( m_Title.empty() )
			{
				m_strError = "title is empty!";
				return;
			}

			//
			// Get the description
			//
			m_Description	= tree.ChildValue( "description", "Description" );
			//
			// Load the addon type
			//
			m_AddonType		= tree.ChildValue( "type", "" );
			{
				Bootil::String::Lower( m_AddonType );

				if ( m_AddonType.empty() )
				{
					m_strError = "type is empty!";
					return;
				}

				//
				// Verify that the addon type is valid by checking it against the list of valids
				//
				if ( !Addon::Tags::TypeExists( m_AddonType ) )
				{
					m_strError = "type isn't a supported type!";
					return;
				}
			}
			//
			// Parse the tags
			//
			{
				Bootil::Data::Tree tags = tree.GetChild( "tags" );

				//
				// Max 2 tags
				//
				if ( tags.Children().size() > 2 )
				{
					m_strError = "too many tags - specify 2 only!";
					return;
				}

				//
				// Collate and check the tags
				//
				BOOTIL_FOREACH( child, tags.Children(), Bootil::Data::Tree::List )
				{
					if (child->Value() == "") continue;

					m_Tags.push_back( child->Value() );
					Bootil::String::Lower( m_Tags.back() );

					if ( !Addon::Tags::TagExists( m_Tags.back() ) )
					{
						m_strError = "tag isn't a supported word!";
						return;
					}					
				}
			}
			//
			// Parse the ignores
			//
			Bootil::Data::Tree ignores = tree.GetChild( "ignore" );
			BOOTIL_FOREACH( child, ignores.Children(), Bootil::Data::Tree::List )
			{
				m_Ignores.push_back( child->Value() );
			}
		}

		void RemoveIgnoredFiles( Bootil::String::List& files, bool quiet )
		{
			// We don't use them directly since the compiler then would create a std::string for every iteration...
			static const Bootil::BString addon_json = "addon.json";
			static const Bootil::BString thumbs_db = "*thumbs.db";
			static const Bootil::BString desktop_ini = "*desktop.ini";
			static const Bootil::BString git = ".git*";
			static const Bootil::BString ds_storage = "*/.DS_Store";

			static Bootil::BString strLow;
			strLow.reserve( 255 );

			BOOTIL_FOREACH_MANUAL( f, files, Bootil::String::List )
			{
				Bootil::Output::Msg("%s\n", (*f).c_str());
				bool bSkipFile = false;

				//
				// Never include our json file!
				//
				if ( *f == addon_json ) bSkipFile = true;

				//
				// Don't include Windows specifiic files
				//
				strLow.assign( *f );
				Addon::GetLower( strLow );

				if ( Addon::CheckWildcard( thumbs_db, strLow ) ) bSkipFile = true;
				if ( Addon::CheckWildcard( ds_storage, strLow ) ) bSkipFile = true;

				// Git stuff
				if ( Addon::CheckWildcard( git, strLow ) ) bSkipFile = true;

				//
				// Don't include OS X metadata files
				//
				if ( *f == ".DS_Store" ) bSkipFile = true;
				if ( Addon::CheckWildcard( ds_storage, *f ) ) bSkipFile = true;

				//
				// Check against our loaded ignores list
				//
				if ( !bSkipFile )
				{
					BOOTIL_FOREACH( ignore, m_Ignores, Bootil::String::List )
					{

						if ( Addon::CheckWildcard( *ignore, *f ) )
						{
							bSkipFile = true;
							break;
						}
					}
				}

				if ( bSkipFile )
				{
					m_IgnoredFiles++;
					if ( !quiet ) Bootil::Output::Msg( "\tIgnored %s\n", f->c_str() );
					f = files.erase( f );
				} else {
					++f; // Manually incrementing it since if we were to increment after a file was ignored/erased we would skip the next element
				}
			}
		}

		//
		// Build a JSON description to store in the GMA
		//
		Bootil::BString BuildDescription()
		{
			Bootil::Data::Tree tree;
			tree.SetChild( "description", GetDescription() );
			tree.SetChild( "type", GetType() );
			Bootil::Data::Tree& tags = tree.GetChild( "tags" );
			BOOTIL_FOREACH( tag, m_Tags, Bootil::String::List )
			{
				tags.AddChild().Value( *tag );
			}

			Bootil::BString strOutput;
			Bootil::Data::Json::Export( tree, strOutput, true );
			//Bootil::Output::Msg( "\n\n%s\n\n", strOutput.c_str() );
			return strOutput;
		}

		const Bootil::BString& GetError() { return m_strError; }
		const Bootil::BString& GetTitle() { return m_Title; }
		const Bootil::BString& GetDescription() { return m_Description; }
		const Bootil::BString& GetType() { return m_AddonType; }


		Bootil::BString			m_strError;

		Bootil::BString			m_Title;
		Bootil::BString			m_Description;
		Bootil::BString			m_AddonType;
		Bootil::String::List	m_Ignores;
		Bootil::String::List	m_Tags;
		int						m_IgnoredFiles;
};



#endif