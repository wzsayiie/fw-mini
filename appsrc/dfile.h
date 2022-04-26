#pragma once

#include <functional>

namespace dash {

void read_file (const char *path, const std::function<void *(int size)> &buffer);
void write_file(const char *path, const void *bytes, int size);

} //end dash.
