
#ifndef GMADD_H
#define GMADD_H
#include "Bootil/Bootil.h"

inline void Pause()
{
#ifdef _WIN32
	// We only need this on Windows
	system( "pause" );
#endif
}

int CreateAddonFile( Bootil::BString strFolder, Bootil::BString strOutfile, bool warnInvalid, bool pauseOnError );
int ExtractAddonFile( Bootil::BString strFile, Bootil::BString strOutPath, bool pauseOnError );

#endif