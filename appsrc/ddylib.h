#pragma once

namespace dash {

struct dylib;

dylib *load_dylib (const char *path);
void  *find_symbol(dylib *lib, const char *name);

} //end dash.
