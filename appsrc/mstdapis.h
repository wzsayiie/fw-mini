#pragma once

#include "mexport.h"
#include "mtypes.h"

MData *MCopyFileContent(MString *path);
void MWriteFile(MString *path, MData *content);

MEXPORT(MCopyFileContent)
MEXPORT(MWriteFile)
