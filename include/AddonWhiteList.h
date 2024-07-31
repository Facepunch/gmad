
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
		static const char* Wildcard[] =
		{
			"lua/*.lua",
			"scenes/*.vcd",
			"particles/*.pcf",
			"resource/fonts/*.ttf",
			"scripts/vehicles/*.txt",
			"resource/localization/*/*.properties",
			"maps/*.bsp",
			"maps/*.lmp",
			"maps/*.nav",
			"maps/*.ain",
			"maps/thumb/*.png",
			"sound/*.wav",
			"sound/*.mp3",
			"sound/*.ogg",
			"materials/*.vmt",
			"materials/*.vtf",
			"materials/*.png",
			"materials/*.jpg",
			"materials/*.jpeg",
			"materials/colorcorrection/*.raw",
			"models/*.mdl",
			"models/*.phy",
			"models/*.ani",
			"models/*.vvd",
			"gamemodes/*/*.txt",
			"gamemodes/*/*.fgd",
			"gamemodes/*/logo.png",
			"gamemodes/*/icon24.png",
			"gamemodes/*/gamemode/*.lua",
			"gamemodes/*/entities/effects/*.lua",
			"gamemodes/*/entities/weapons/*.lua",
			"gamemodes/*/entities/entities/*.lua",
			"gamemodes/*/backgrounds/*.png",
			"gamemodes/*/backgrounds/*.jpg",
			"gamemodes/*/backgrounds/*.jpeg",
			"gamemodes/*/content/models/*.mdl",
			"gamemodes/*/content/models/*.phy",
			"gamemodes/*/content/models/*.ani",
			"gamemodes/*/content/models/*.vvd",
			"gamemodes/*/content/materials/*.vmt",
			"gamemodes/*/content/materials/*.vtf",
			"gamemodes/*/content/materials/*.png",
			"gamemodes/*/content/materials/*.jpg",
			"gamemodes/*/content/materials/*.jpeg",
			"gamemodes/*/content/materials/colorcorrection/*.raw",
			"gamemodes/*/content/scenes/*.vcd",
			"gamemodes/*/content/particles/*.pcf",
			"gamemodes/*/content/resource/fonts/*.ttf",
			"gamemodes/*/content/scripts/vehicles/*.txt",
			"gamemodes/*/content/resource/localization/*/*.properties",
			"gamemodes/*/content/maps/*.bsp",
			"gamemodes/*/content/maps/*.nav",
			"gamemodes/*/content/maps/*.ain",
			"gamemodes/*/content/maps/thumb/*.png",
			"gamemodes/*/content/sound/*.wav",
			"gamemodes/*/content/sound/*.mp3",
			"gamemodes/*/content/sound/*.ogg",

			// static version of the data/ folder
			// (because you wouldn't be able to modify these)
			"data_static/*.txt",
			"data_static/*.dat",
			"data_static/*.json",
			"data_static/*.xml",
			"data_static/*.csv",
			"data_static/*.dem",
			"data_static/*.vcd",

			"data_static/*.vtf",
			"data_static/*.vmt",
			"data_static/*.png",
			"data_static/*.jpg",
			"data_static/*.jpeg",

			"data_static/*.mp3",
			"data_static/*.wav",
			"data_static/*.ogg",

			NULL
		};

		//
		// Call on a filename including relative path to determine
		// whether file is allowed to be in the addon.
		//
		inline bool Check( const Bootil::BString& strname )
		{
			bool bValid = false;

			for ( int i = 0;; i++ )
			{
				if ( bValid || WhiteList::Wildcard[i] == NULL ) break;

				bValid = Bootil::String::Test::Wildcard( Wildcard[i], strname );
			}

			return bValid;
		}

	}
}

#endif
