#pragma once

#include "mhostapi.h"

M_FUNC_EXPORT MData   *MCopyDataFromFile     (MString *path) M_META(MCopyDataFromFile     );
M_FUNC_EXPORT MString *MCopyStringFromFile   (MString *path) M_META(MCopyStringFromFile   );
M_FUNC_EXPORT MString *MCopyStringFromBundle (MString *path) M_META(MCopyStringFromBundle );
M_FUNC_EXPORT MImage  *MCreateImageFromFile  (MString *path) M_META(MCreateImageFromFile  );
M_FUNC_EXPORT MImage  *MCreateImageFromBundle(MString *path) M_META(MCreateImageFromBundle);

M_FUNC_EXPORT void MWriteDataToFile     (MString *path, MData   *data  ) M_META(MWriteDataToFile     );
M_FUNC_EXPORT void MWriteU8StringToFile (MString *path, MString *string) M_META(MWriteU8StringToFile );
M_FUNC_EXPORT void MWriteU16StringToFile(MString *path, MString *string) M_META(MWriteU16StringToFile);
