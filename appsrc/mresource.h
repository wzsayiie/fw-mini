#pragma once

#include "mhostapi.h"

M_FUNC_EXPORT MData   *MCopyDataFromFile     (MString *path) MFUNC_META(MCopyDataFromFile     );
M_FUNC_EXPORT MString *MCopyStringFromFile   (MString *path) MFUNC_META(MCopyStringFromFile   );
M_FUNC_EXPORT MString *MCopyStringFromBundle (MString *path) MFUNC_META(MCopyStringFromBundle );
M_FUNC_EXPORT MImage  *MCreateImageFromFile  (MString *path) MFUNC_META(MCreateImageFromFile  );
M_FUNC_EXPORT MImage  *MCreateImageFromBundle(MString *path) MFUNC_META(MCreateImageFromBundle);

M_FUNC_EXPORT void MWriteDataToFile     (MString *path, MData   *data  ) MFUNC_META(MWriteDataToFile     );
M_FUNC_EXPORT void MWriteU8StringToFile (MString *path, MString *string) MFUNC_META(MWriteU8StringToFile );
M_FUNC_EXPORT void MWriteU16StringToFile(MString *path, MString *string) MFUNC_META(MWriteU16StringToFile);
