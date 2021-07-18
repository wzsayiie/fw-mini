#pragma once

#include "mtypes.h"

MData *MCopyFileContent(MString *path);
void MWriteFile(MString *path, MData *content);

MFUNC_EXPORT(MCopyFileContent)
MFUNC_EXPORT(MWriteFile)
