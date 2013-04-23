
#ifndef GMADD_H
#define GMADD_H
#include "Bootil/Bootil.h"

int CreateAddonFile( Bootil::BString strFolder, Bootil::BString strOutfile, bool warnInvalid );
int ExtractAddonFile( Bootil::BString strFile, Bootil::BString strOutPath );

#endif