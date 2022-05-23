#pragma once

#include "denviron.h"

#if DASH_COMPILER_MSC
    #define dash_exportable __declspec(dllexport)

    //NOTE: if a dll exportable class inherits a template class,
    //or contains template class object as members, will cause this warning.
    #pragma warning(disable: 4251)
#else
    #define dash_exportable __attribute((visibility("default")))
#endif
