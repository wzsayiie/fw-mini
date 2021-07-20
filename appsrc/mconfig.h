#pragma once

#include "menviron.h"

//functions that need to be called by the host.
#define MFUNC_HOST extern "C"

//basic functions for realizing the export function.
#if MCPL_CL
    #define MFUNC_BASE extern "C" __declspec(dllexport)
#else
    #define MFUNC_BASE extern "C" __attribute((visibility("default")))
#endif

//exported functions that can be called dynamically.
#ifndef MFUNC_EXPORT
    #define MFUNC_EXPORT(name)
#endif
