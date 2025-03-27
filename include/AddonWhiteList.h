
#ifndef ADDONWHITELIST_H
#define ADDONWHITELIST_H

#include "Bootil/Bootil.h"

namespace Addon
{
	namespace WhiteList
	{
		// Entries that start with ! invalidate all previous matches
		// Caveat: Order of these now matters
		// Ideally Bootil would support something like /**/ for "only one folder" or "no slashes" (or backwards to mimic gitignore)
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

			"models/*.vtx",
			"!models/*.sw.vtx", // These variations are unused by the game
			"!models/*.360.vtx",
			"!models/*.xbox.vtx",

			"gamemodes/*/*.txt",
			"!gamemodes/*/*/*.txt", // Only in the root gamemode folder please!
			"gamemodes/*/*.fgd",
			"!gamemodes/*/*/*.fgd",

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

			"gamemodes/*/content/models/*.vtx",
			"!gamemodes/*/content/models/*.sw.vtx",
			"!gamemodes/*/content/models/*.360.vtx",
			"!gamemodes/*/content/models/*.xbox.vtx",

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
			// We only allow filetypes here that are not already allowed above
			"data_static/*.txt",
			"data_static/*.dat",
			"data_static/*.json",
			"data_static/*.xml",
			"data_static/*.csv",

			"shaders/*.vcs",

			NULL
		};

		//
		// Call on a filename including relative path to determine whether file is allowed to be in the addon.
		// This whitelist only serves to warn about bad files at upload stage - the game has its own whitelist.
		//
		inline bool Check( const Bootil::BString& strname )
		{
			bool bValid = false;

			for ( int i = 0;; i++ )
			{
				if ( WhiteList::Wildcard[i] == NULL ) break;

				if ( !bValid ) bValid = Bootil::String::Test::Wildcard( Wildcard[i], strname );

				if ( Bootil::String::Test::StartsWith( Wildcard[i], "!" ) )
				{
					Bootil::BString wildCard =  Wildcard[i];
					wildCard = wildCard.substr( 1 );
					if ( Bootil::String::Test::Wildcard( wildCard, strname ) )
					{
						bValid = false;
					}
				}
			}

			return bValid;
		}

	}
}

#endif
