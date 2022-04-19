#include "ddylib.h"
#include <map>
#include <string>
#include "denviron.h"
#include "dcustom.h"

#if DASH_OS_WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

namespace dash {

static lazy_var<std::map<std::string, dylib *>> s_libs;

dylib *load_dylib(const char *path) {
    if (!path || !*path) {
        return nullptr;
    }

    auto target = s_libs->find(path);
    if (target != s_libs->end()) {
        return target->second;
    }

#if DASH_OS_WIN32
    auto lib = (dylib *)LoadLibraryA(path);
#else
    auto lib = (dylib *)dlopen(path, RTLD_NOW);
#endif

    if (lib) {
        s_libs->insert({ path, lib });
    }
    return lib;
}

void *find_symbol(dylib *lib, const char *name) {
    if (lib && name && *name) {
    #if DASH_OS_WIN32
        return GetProcAddress((HMODULE)lib, name);
    #else
        return dlsym(lib, name);
    #endif
    }
    return nullptr;
}

} //end dash.
