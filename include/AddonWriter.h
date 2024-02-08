
#ifndef ADDONWRITER_H
#define ADDONWRITER_H

#include <stdint.h>
#include "Bootil/Bootil.h"
#include "AddonFormat.h"

//
//
//
//
namespace Addon
{
	class Writer : public Bootil::AutoBuffer
	{
		public:

			Writer( Bootil::BString strOutput, bool nobuffer )
			{
				m_usebuffer = !nobuffer;
				m_writeblocks = false;
				m_output = strOutput;
				m_written = 0;

				remove( strOutput.c_str() );
				m_file.open( strOutput, std::ios::out | std::ios::binary | std::ios::app );
				if ( !m_file.is_open() )
				{
					Bootil::Output::Error( "Failed to open %s!\n", strOutput );
				}
			}

			uint64_t GetWritten()
			{
				if ( m_usebuffer )
					return AutoBuffer::GetWritten() + m_written;

				return m_written;
			}

			bool EnsureCapacity( uint64_t iSize )
			{
				if ( m_usebuffer )
				{
					uint64_t max = INT_MAX / 1.5; // ~1.4GB will be ready for the buffer. If we try to use more, it will fail to allocate it for some reason.
					if ( iSize > max )
					{
						m_writeblocks = true;
						iSize = max;
					}

					return AutoBuffer::EnsureCapacity( iSize );
				}

				return true;
			}

			void Reset()
			{
				m_iPos = 0;
				m_iWritten = 0;
			}

			void WriteBuffer( const Bootil::Buffer & bufferOut )
			{
				if ( m_usebuffer )
				{
					if ( m_writeblocks && ( this->GetSize() - AutoBuffer::GetWritten() ) <= bufferOut.GetWritten() )
					{
						m_file.write( reinterpret_cast<char*>( this->GetBase() ), AutoBuffer::GetWritten() );
						m_written += AutoBuffer::GetWritten();
						Reset();

						if ( bufferOut.GetWritten() >= this->GetSize() )
						{
							m_file.write( reinterpret_cast<char*>( bufferOut.GetBase() ), bufferOut.GetWritten() );
							m_written += bufferOut.GetWritten();
						} 
						else
						{
							AutoBuffer::WriteBuffer( bufferOut );
						}
					}
					else
					{
						AutoBuffer::WriteBuffer( bufferOut );
					}
				}
				else 
				{
					if ( AutoBuffer::GetWritten() > 0 )
					{
						m_file.write( reinterpret_cast<char*>( this->GetBase() ), AutoBuffer::GetWritten() );
						m_written += AutoBuffer::GetWritten();
						Reset();
					}

					m_file.write( reinterpret_cast<char*>( bufferOut.GetBase() ), bufferOut.GetWritten() );
					m_written += bufferOut.GetWritten();
				}
			}

			bool WriteFile()
			{
				if ( m_usebuffer )
				{
					if ( m_writeblocks )
					{
						m_written += AutoBuffer::GetWritten();
						m_file.write( reinterpret_cast<char*>( this->GetBase() ), AutoBuffer::GetWritten() );
						return true;
					}
					else
					{
						return Bootil::File::Write( m_output, *(Bootil::AutoBuffer*)this );
					}
				}

				Close();

				return true;
			}

			uint32_t CRC()
			{
				if ( !m_usebuffer || m_writeblocks )
					return 0;

				return Bootil::Hasher::CRC32::Easy( this->GetBase(), this->GetWritten() );
			}

			void Close()
			{
				if ( m_usebuffer )
				{
					AutoBuffer::Clear();
				}
				else
				{
					m_file.close();
				}
			}

			Bootil::BString FormatSize() // Bootil::String::Format::Memory has a 2GB limit.
			{
				uint64_t iBytes = GetWritten();
				float gb = iBytes / 1024.0f / 1024.0f / 1024.0f;
				if ( gb >= 1.0 )
				{
					return Bootil::String::Format::Print( "%.1f GB", gb );
				}

				float mb = iBytes / 1024.0f / 1024.0f;
				if ( mb >= 1.0 )
				{
					return Bootil::String::Format::Print( "%.1f MB", mb );
				}

				float kb = iBytes / 1024.0f;
				if ( kb >= 1.0 )
				{
					return Bootil::String::Format::Print( "%.1f KB", kb );
				}

				return Bootil::String::Format::Print( "%i B", iBytes );
			}

		private:
			Bootil::BString			m_output;
			std::ofstream			m_file;
			bool					m_usebuffer;
			bool					m_writeblocks;
			uint64_t				m_written;
	};
}

#endif