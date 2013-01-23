
#ifndef ADDON_FORMAT_H
#define ADDON_FORMAT_H
#include "Bootil/Bootil.h"


namespace Addon 
{
	static const char*	Ident							= "GMAD";
	static const char	Version							= 3;
	static const unsigned int AppID						= 4000;
	static const unsigned int CompressionSignature		= 0xBEEFCACE;
	

	struct Header 
	{
		char			Ident[4];
		char			Version;
	};

	struct FileEntry
	{
		Bootil::BString	strName;
		long long		iSize;
		unsigned long	iCRC;
		unsigned int	iFileNumber;
		long long		iOffset;

		typedef std::list< FileEntry > List;
	};

	//
	// This is the position in the file containing a 64 bit unsigtned int that represents the file's age
	// It's basically the time it was uploaded to Steam - and is set on download/extraction from steam.
	//
	static unsigned int			TimestampOffset			= sizeof(Addon::Header) + sizeof( unsigned long long );
}
#endif