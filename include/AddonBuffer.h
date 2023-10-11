
#ifndef ADDONBUFFER_H
#define ADDONBUFFER_H

#include "Bootil/Bootil.h"

namespace Addon
{
	class AddonBuffer : public Bootil::AutoBuffer
	{
		public:
			bool EnsureCapacity( unsigned int iSize ) // Same as AutoBuffer::EnsureCapacity, but the 500mb limit was removed.
			{
				if ( iSize <= m_iSize ) { return true; }

				if ( !m_pData )
				{
					m_pData = malloc( iSize );
					if ( !m_pData )
					{
						return false;
					}
				}
				else
				{
					void* pData = realloc( m_pData, iSize );

					if ( pData )
					{
						m_pData = pData;
					}
					else
					{
						pData = malloc( iSize );

						if ( !pData ) { return false; }

						memcpy( pData, m_pData, m_iSize );
						free( m_pData );
						m_pData = pData;
					}

					if ( !m_pData )
					{
						return false;
					}
				}

				m_iSize = iSize;
				return true;
			}

	};

}

#endif