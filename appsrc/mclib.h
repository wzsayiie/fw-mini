#pragma once

#include "mtypes.h"

MData   *MCopyFileContent (MString *path);
MString *MCopyFileString  (MString *path);
MString *MCopyBundleString(MString *path);

void MWriteFile   (MString *path, MData   *content);
void MWriteU8File (MString *path, MString *string );
void MWriteU16File(MString *path, MString *string );

MFUNC_EXPORT(MCopyFileContent )
MFUNC_EXPORT(MCopyFileString  )
MFUNC_EXPORT(MCopyBundleString)
MFUNC_EXPORT(MWriteFile       )
MFUNC_EXPORT(MWriteU8File     )
MFUNC_EXPORT(MWriteU16File    )
