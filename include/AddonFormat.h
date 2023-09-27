
#ifndef ADDON_FORMAT_H
#define ADDON_FORMAT_H
#include <stdint.h>
#include "Bootil/Bootil.h"


namespace Addon
{
	static const char*	Ident							= "GMAD";
	static const char	Version							= 3;
	static const uint32_t AppID							= 4000;
	static const uint32_t CompressionSignature			= 0xBEEFCACE;


	struct Header
	{
		char			Ident[4];
		char			Version;
	};

	struct FileEntry
	{
		Bootil::BString	strName;
		int64_t		iSize;
		uint32_t	iCRC;
		uint32_t	iFileNumber;
		int64_t		iOffset;

		typedef std::list< FileEntry > List;
	};

	//
	// A list of tags that are available to set
	//
	namespace Tags
	{
		//
		// Only one of these
		//
		static const char* Type[] =
		{
			"gamemode",
			"map",
			"weapon",
			"vehicle",
			"npc",
			"entity",
			"tool",
			"effects",
			"model",
			"servercontent"
		};

		static const short TypesCount = sizeof(Type) / sizeof(void*);

		inline bool TypeExists( const Bootil::BString& strName )
		{
			for ( int i = 0; i < TypesCount; i++ )
			{
				if ( strName == Addon::Tags::Type[i] )
					return true;
			}

			return false;
		}


		//
		// Up to two of these
		//
		static const char* Misc[] =
		{
			"fun",
			"roleplay",
			"scenic",
			"movie",
			"realism",
			"cartoon",
			"water",
			"comic",
			"build"
		};

		static const short MiscCount = sizeof(Misc) / sizeof(void*);

		inline bool TagExists( const Bootil::BString& strName )
		{
			for ( int i = 0; i < MiscCount; i++ )
			{
				if ( strName == Addon::Tags::Misc[i] )
					return true;
			}

			return false;
		}
	};

	//
	// This is the position in the file containing a 64 bit unsigned int that represents the file's age
	// It's basically the time it was uploaded to Steam - and is set on download/extraction from steam.
	//
	static uint32_t			TimestampOffset			= sizeof( Addon::Header ) + sizeof( uint64_t );
}
#endif