#pragma once

#include "mhostapi.h"

MFUNC_EXPORT MData   *MCopyDataFromFile     (MString *path) MFUNC_META(MCopyDataFromFile     );
MFUNC_EXPORT MString *MCopyStringFromFile   (MString *path) MFUNC_META(MCopyStringFromFile   );
MFUNC_EXPORT MString *MCopyStringFromBundle (MString *path) MFUNC_META(MCopyStringFromBundle );
MFUNC_EXPORT MImage  *MCreateImageFromFile  (MString *path) MFUNC_META(MCreateImageFromFile  );
MFUNC_EXPORT MImage  *MCreateImageFromBundle(MString *path) MFUNC_META(MCreateImageFromBundle);

MFUNC_EXPORT void MWriteDataToFile     (MString *path, MData   *data  ) MFUNC_META(MWriteDataToFile     );
MFUNC_EXPORT void MWriteU8StringToFile (MString *path, MString *string) MFUNC_META(MWriteU8StringToFile );
MFUNC_EXPORT void MWriteU16StringToFile(MString *path, MString *string) MFUNC_META(MWriteU16StringToFile);
