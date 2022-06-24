#pragma once

#include "dexport.h"
#include "dstd.h"

namespace dash {

dash_exportable void read_file (const char *path, const std::function<void *(int size)> &buffer);
dash_exportable void write_file(const char *path, const void *bytes, int size);

} //end dash.
