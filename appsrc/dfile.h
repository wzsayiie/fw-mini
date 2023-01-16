#pragma once

#include <functional>
#include "dexport.h"

namespace dash {

d_exportable void read_file (const char *path, const std::function<void *(int size)> &buffer);
d_exportable void write_file(const char *path, const void *bytes, int size);

} //end dash.
