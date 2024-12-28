#ifndef ADDONUTIL_H
#define ADDONUTIL_H

#include "Bootil/Bootil.h"

// These should probably be added to Bootil itself, but since bootil didn't have any changes for 5+ years I expect nothing.

// Iterator but you manually increment the iterator
#define BOOTIL_FOREACH_MANUAL( varname, arrayname, arraytype ) for ( arraytype::iterator varname = arrayname.begin(); varname != arrayname.end(); )

namespace Addon
{
	//
	// From this URL: http://www.codeproject.com/KB/string/wildcmp.aspx
	// Written by Jack Handy - jakkhandy@hotmail.com
	// 

	inline bool globber( const char *wild, const char *string ) 
	{
		const char *cp = 0, *mp = 0;

		while ((*string) && (*wild != '*')) {
			if ((*wild != *string) && (*wild != '?')) {
				return false;
			}
			wild++;
			string++;
		}

		while (*string) {
			if (*wild == '*') {
				if (!*++wild) {
					return true;
				}
				mp = wild;
				cp = string+1;
			} else if ((*wild == *string) || (*wild == '?')) {
				wild++;
				string++;
			} else {
				wild = mp;
				string = cp++;
			}
		}

		while (*wild == '*') {
			wild++;
		}
		return !*wild;
	}

	// This is faster than Bootil::String::Test::Wildcard since were passing a reference instead of creating a copy.
	inline bool CheckWildcard( const Bootil::BString& wildCard, const Bootil::BString& hayStack )
	{
		return globber( wildCard.c_str(), hayStack.c_str() );
	}

	// NOTE: This directly modifies the given string instead of making a copy.
	inline void GetLower( Bootil::BString& fileName )
	{
		std::transform( fileName.begin(), fileName.end(), fileName.begin(), ::tolower );
	}
}
#endif