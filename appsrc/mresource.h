#pragma once

#include "mhostapi.h"

M_FUNC_EXPORT MData   *MCopyDataFromFile     (MString *path) M_META(MCopyDataFromFile     , "args:path");
M_FUNC_EXPORT MString *MCopyStringFromFile   (MString *path) M_META(MCopyStringFromFile   , "args:path");
M_FUNC_EXPORT MString *MCopyStringFromBundle (MString *path) M_META(MCopyStringFromBundle , "args:path");
M_FUNC_EXPORT MImage  *MCreateImageFromFile  (MString *path) M_META(MCreateImageFromFile  , "args:path");
M_FUNC_EXPORT MImage  *MCreateImageFromBundle(MString *path) M_META(MCreateImageFromBundle, "args:path");

M_FUNC_EXPORT bool MWriteDataToFile     (MString *path, MData   *data  ) M_META(MWriteDataToFile     , "args:path,dat");
M_FUNC_EXPORT bool MWriteU8StringToFile (MString *path, MString *string) M_META(MWriteU8StringToFile , "args:path,str");
M_FUNC_EXPORT bool MWriteU16StringToFile(MString *path, MString *string) M_META(MWriteU16StringToFile, "args:path,str");

M_FUNC_EXPORT void    MMakeDirectores (MString *path) M_META(MMakeDirectores , "args:path");
M_FUNC_EXPORT MArray *MCopyDirSubitems(MString *path) M_META(MCopyDirSubitems, "args:path");
M_FUNC_EXPORT void    MRemovePath     (MString *path) M_META(MRemovePath     , "args:path");
M_FUNC_EXPORT bool    MPathExists     (MString *path) M_META(MPathExists     , "args:path");
M_FUNC_EXPORT bool    MDirectoryExists(MString *path) M_META(MDirectoryExists, "args:path");
M_FUNC_EXPORT bool    MFileExists     (MString *path) M_META(MFileExists     , "args:path");
