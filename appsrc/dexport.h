#pragma once

#include "denviron.h"

#if D_COMPILER_MSC
    #define d_exportable __declspec(dllexport)

    //NOTE: if a dll exportable class inherits a template class,
    //or contains template class object as members, will cause this warning.
    #pragma warning(disable: 4251)
#else
    #define d_exportable __attribute((visibility("default")))
#endif
