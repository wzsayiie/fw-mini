#pragma once

#include "denviron.h"

#if DASH_COMPILER_MSC
    #define dash_exportable __declspec(dllexport)
#else
    #define dash_exportable __attribute((visibility("default")))
#endif
