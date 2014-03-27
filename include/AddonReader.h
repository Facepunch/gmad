
#ifndef ADDONREADER_H
#define ADDONREADER_H

#include <stdint.h>
#include "Bootil/Bootil.h"
#include "AddonFormat.h"


//
//
//
//
namespace Addon
{
	class Reader
	{
		public:

			Reader()
			{
				Clear();
			}

			//
			// Load an addon (call Parse after this succeeds)
			//
			bool ReadFromFile( Bootil::BString strName )
			{
				m_buffer.Clear();
				return Bootil::File::Read( strName, m_buffer );
			}

			//
			// Parse the addon into this class
			//
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
				// Format Version
				//
				m_fmtversion = m_buffer.ReadType<char>();

				if ( m_fmtversion > Addon::Version )
					return false;

				m_buffer.ReadType<uint64_t>(); // steamid
				m_buffer.ReadType<uint64_t>(); // timestamp

				//
				// Required content (not used at the moment, just read out)
				//
				if ( m_fmtversion > 1 )
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
				m_buffer.ReadType<int32_t>();
				//
				// File index
				//
				int32_t iFileNumber = 1;
				int32_t iOffset = 0;

				while ( m_buffer.ReadType<uint32_t>() != 0 )
				{
					Addon::FileEntry entry;
					entry.strName		= m_buffer.ReadString();
					entry.iSize			= m_buffer.ReadType<int64_t>();
					entry.iCRC			= m_buffer.ReadType<uint32_t>();
					entry.iOffset		= iOffset;
					entry.iFileNumber	= iFileNumber;
					m_index.push_back( entry );
					iOffset += entry.iSize;
					iFileNumber++;
				}

				m_fileblock = m_buffer.GetPos();
				//
				// Try to parse the description
				//
				Bootil::Data::Tree json;

				if ( Bootil::Data::Json::Import( json, m_desc.c_str() ) )
				{
					m_desc = json.ChildValue( "description" );
					m_type = json.ChildValue( "type" );
					Bootil::Data::Tree& tags = json.GetChild( "tags" );
					BOOTIL_FOREACH( tag, tags.Children(), Bootil::Data::Tree::List )
					{
						m_tags.push_back( tag->Value() );
					}
				}

				return true;
			}

			//
			// Return the FileEntry for a FileID
			//
			bool GetFile( uint32_t iFileID, Addon::FileEntry& outfile )
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

			//
			// Read a fileid from the addon into the buffer
			//
			bool ReadFile( uint32_t iFileID, Bootil::Buffer& buffer )
			{
				Addon::FileEntry file;

				if ( !GetFile( iFileID, file ) ) return false;

				buffer.Write( m_buffer.GetBase( m_fileblock + file.iOffset ), file.iSize );
				return true;
			}

			//
			// Clears the addon's variables, frees all memory
			//
			void Clear()
			{
				m_buffer.Clear();
				m_fmtversion = 0;
				m_name.clear();
				m_author.clear();
				m_desc.clear();
				m_index.clear();
				m_type.clear();
				m_fileblock = 0;
				m_tags.clear();
			}

			const Addon::FileEntry::List& GetList() { return m_index; }
			uint32_t GetFormatVersion() { return m_fmtversion; }
			const Bootil::Buffer& GetBuffer() { return m_buffer; }
			Bootil::BString Title() { return m_name; }
			Bootil::BString Description() { return m_desc; }
			Bootil::BString Author() { return m_author; }
			const Bootil::BString& Type() { return m_type; }
			Bootil::String::List& Tags() { return m_tags; }

		protected:

			Bootil::AutoBuffer		m_buffer;
			char					m_fmtversion;
			Bootil::BString			m_name;
			Bootil::BString			m_author;
			Bootil::BString			m_desc;
			Bootil::BString			m_type;
			Addon::FileEntry::List	m_index;
			uint32_t			m_fileblock;

			Bootil::String::List	m_tags;

	};

}

#endif
