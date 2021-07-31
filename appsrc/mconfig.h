#pragma once

#include "menviron.h"

//functions that need to be called by the host.
#define MFUNC_HOST extern "C"

//exported functions.
#if MCPL_CL
    #define MFUNC_EXPORT extern "C" __declspec(dllexport)
#else
    #define MFUNC_EXPORT extern "C" __attribute((visibility("default")))
#endif

//functions recorded meta data.
#ifndef MFUNC_META
    #define MFUNC_META(name)
#endif
