#pragma once

#include "menviron.h"

//------------------------------------------------------------------------------
//available types enumeration:

#define MEnumId(n) (((int)n[0]) | ((int)n[1] << 8) | ((int)n[2] << 16))

typedef int MType;

enum {
    MType_void     = MEnumId("vid"),    //void.
    MType_bool     = MEnumId("bol"),    //bool.
    MType_int      = MEnumId("int"),    //int.
    MType_float    = MEnumId("flt"),    //float.
    MType_pointer  = MEnumId("ptr"),    //uint8_t *.
    MType_s8ptr    = MEnumId("s08"),    //char *.
    MType_s16ptr   = MEnumId("s16"),    //char16_t *.
    MType_MObject  = MEnumId("Obj"),    //MObject.
    MType_MBool    = MEnumId("Bol"),    //MBool.
    MType_MInt     = MEnumId("Int"),    //MInt.
    MType_MFloat   = MEnumId("Flt"),    //MFloat.
    MType_MPointer = MEnumId("Ptr"),    //MPointer.
    MType_MString  = MEnumId("Str"),    //MString.
    MType_MLambda  = MEnumId("Lmd"),    //MLamdba.
    MType_MData    = MEnumId("Dat"),    //MData.
    MType_MArray   = MEnumId("Arr"),    //MArray.
    MType_MImage   = MEnumId("Img"),    //MImage.
    MType_MSpecial = MEnumId("Spc"),    //MSpecial.
};

//------------------------------------------------------------------------------
//function meta information:

const int MFuncMaxArgCount = 4;

struct _MFuncMeta {
    void *address   = nullptr;
    MType retType   = 0;
    bool  retRetain = false;
    int   argCount  = 0;

    MType argTypes[MFuncMaxArgCount] = {0};
};

//------------------------------------------------------------------------------
//macros for function descritpion:

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
