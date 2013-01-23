
#ifndef ADDONWHITELIST_H
#define ADDONWHITELIST_H

#include "Bootil/Bootil.h"

//
// 
//
//
namespace Addon 
{
	namespace WhiteList 
	{
		static const char * Wildcard[] =
		{
			"maps/*.bsp",
			"maps/*.png",
			"maps/*.nav",
			"maps/*.ain",
			"sound/*.wav",
			"sound/*.mp3",
			"lua/*.lua",
			"materials/*.vmt",
			"materials/*.vtf",
			"materials/*.png",
			"models/*.mdl",
			"models/*.vtx",
			"models/*.phy",
			"models/*.ani",
			"models/*.vvd",
			"gamemodes/*.txt",
			"gamemodes/*.lua",
			"scenes/*.vcd",
			"particles/*.pcf",
			"gamemodes/*/backgrounds/*.jpg",
			"gamemodes/*/icon24.png",
			"gamemodes/*/logo.png",
			"scripts/vehicles/*.txt",
			"resource/fonts/*.ttf",
			NULL
		};

		//
		// Call on a filename including relative path to determine 
		// whether file is allowed to be in the addon.
		//
		inline bool Check( const Bootil::BString& strname )
		{
			bool bValid = false;
			for ( int i=0;; i++ )
			{
				if ( bValid || WhiteList::Wildcard[i] == NULL ) break;
				bValid = Bootil::String::Test::Wildcard( Wildcard[i], strname );
			}

			return bValid;
		}

	}
}

#endif