
#ifndef ADDONREADER_H
#define ADDONREADER_H

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

			//
			// Return the FileEntry for a FileID
			//
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

			//
			// Read a fileid from the addon into the buffer
			//
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

}

#endif