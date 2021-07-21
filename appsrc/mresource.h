#pragma once

#include "mtypes.h"

MData   *MCopyDataFromFile     (MString *path);
MString *MCopyStringFromFile   (MString *path);
MString *MCopyStringFromBundle (MString *path);
MImage  *MCreateImageFromFile  (MString *path);
MImage  *MCreateImageFromBundle(MString *path);

void MWriteDataToFile     (MString *path, MData   *data  );
void MWriteU8StringToFile (MString *path, MString *string);
void MWriteU16StringToFile(MString *path, MString *string);

MFUNC_EXPORT(MCopyDataFromFile     )
MFUNC_EXPORT(MCopyStringFromFile   )
MFUNC_EXPORT(MCopyStringFromBundle )
MFUNC_EXPORT(MCreateImageFromFile  )
MFUNC_EXPORT(MCreateImageFromBundle)
MFUNC_EXPORT(MWriteDataToFile      )
MFUNC_EXPORT(MWriteU8StringToFile  )
MFUNC_EXPORT(MWriteU16StringToFile )
