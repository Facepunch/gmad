
#include "gmadd.h"
#include "AddonWhiteList.h"
#include "AddonFormat.h"

using namespace Bootil;

namespace ExtractAddon
{

}

class GModAddon
{
	public:

		GModAddon()
		{

		}

		bool ReadFromFile( Bootil::BString strName )
		{
			m_buffer.Clear();

			return File::Read( strName, m_buffer );
		}

		bool Parse()
		{
			m_buffer.SetPos( 0 );

			// Ident
			if ( m_buffer.ReadType<char>() != 'G' ||
				 m_buffer.ReadType<char>() != 'M' ||
				 m_buffer.ReadType<char>() != 'A' ||
				 m_buffer.ReadType<char>() != 'D' )
			{
				return false;
			}

			//
			// Version
			//
			m_version = m_buffer.ReadType<char>();
			if ( m_version > Addon::Version )
				return false;

			m_buffer.ReadType<unsigned long long>(); // steamid
			m_buffer.ReadType<unsigned long long>(); // timestamp

			//
			// Required content (not used at the moment, just read out)
			//
			if ( m_version > 1 )
			{
				Bootil::BString strContent = m_buffer.ReadString();

				while ( !strContent.empty() )
				{
					strContent = m_buffer.ReadString();
				}
			}

			m_name		= m_buffer.ReadString();
			m_desc		= m_buffer.ReadString();
			m_author	= m_buffer.ReadString();

			//
			// Addon version - unused
			//
			m_buffer.ReadType<int>();

			//
			// File index
			//
			int iFileNumber = 1;
			int iOffset = 0;
			while ( m_buffer.ReadType<unsigned int>() != 0 )
			{
				Addon::FileEntry entry;

				entry.strName		= m_buffer.ReadString();
				entry.iSize			= m_buffer.ReadType<long long>();
				entry.iCRC			= m_buffer.ReadType<unsigned long>();
				entry.iOffset		= iOffset;
				entry.iFileNumber	= iFileNumber;

				m_index.push_back( entry );

				iOffset += entry.iSize;
				iFileNumber++;
			}

			m_fileblock = m_buffer.GetPos();

			return true;
		}

		bool GetFile( unsigned int iFileID, Addon::FileEntry& outfile )
		{
			BOOTIL_FOREACH( file, m_index, Addon::FileEntry::List )
			{
				if ( file->iFileNumber == iFileID )
				{
					outfile = *file;
					return true;
				}
			}

			return false;
		}

		bool ReadFile( unsigned int iFileID, Bootil::Buffer& buffer )
		{
			Addon::FileEntry file;
			if ( !GetFile( iFileID, file ) ) return false;

			buffer.Write( m_buffer.GetBase( m_fileblock + file.iOffset ), file.iSize );
			return true;
		}

		const Addon::FileEntry::List& GetList(){ return m_index; }

	protected:

		Bootil::AutoBuffer		m_buffer;
		char					m_version;
		Bootil::BString			m_name;
		Bootil::BString			m_author;
		Bootil::BString			m_desc;
		Addon::FileEntry::List	m_index;
		unsigned long			m_fileblock;
};


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

	GModAddon addon;

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