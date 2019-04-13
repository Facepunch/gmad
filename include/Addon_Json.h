
#ifndef ADDON_JSON_H
#define ADDON_JSON_H

class CAddonJson
{
	public:

		CAddonJson( Bootil::BString strInfoFile )
		{
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

		void RemoveIgnoredFiles( Bootil::String::List& files )
		{
			Bootil::String::List old_files = files;
			files.clear();
			BOOTIL_FOREACH( f, old_files, Bootil::String::List )
			{
				bool bSkipFile = false;

				//
				// Never include our json file!
				//
				if ( *f == "addon.json" ) continue;

				//
				// Don't include Windows config file
				//
				if ( *f == "desktop.ini" ) continue;

				//
				// Don't include OS X metadata files
				//
				if ( *f == ".DS_Store" ) continue;
				if ( Bootil::String::Test::Wildcard( "*/.DS_Store", *f ) ) continue;

				//
				// Check against our loaded ignores list
				//
				BOOTIL_FOREACH( ignore, m_Ignores, Bootil::String::List )
				{
					if ( Bootil::String::Test::Wildcard( *ignore, *f ) )
					{
						bSkipFile = true;
						break;
					}
				}

				if ( !bSkipFile )
					files.push_back( *f );
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
};



#endif