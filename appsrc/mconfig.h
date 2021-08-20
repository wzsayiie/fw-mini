#pragma once

#include "menviron.h"

//------------------------------------------------------------------------------
//available types enumeration:

#define MEnumId(n) (((int)n[0]) | ((int)n[1] << 8) | ((int)n[2] << 16))

typedef int MType;

const MType MType_void     = MEnumId("vid");    //void.
const MType MType_bool     = MEnumId("bol");    //bool.
const MType MType_int      = MEnumId("int");    //int.
const MType MType_float    = MEnumId("flt");    //float.
const MType MType_pointer  = MEnumId("ptr");    //uint8_t *.
const MType MType_s8ptr    = MEnumId("s08");    //char *.
const MType MType_s16ptr   = MEnumId("s16");    //char16_t *.
const MType MType_MObject  = MEnumId("Obj");    //MObject.
const MType MType_MBool    = MEnumId("Bol");    //MBool.
const MType MType_MInt     = MEnumId("Int");    //MInt.
const MType MType_MFloat   = MEnumId("Flt");    //MFloat.
const MType MType_MPointer = MEnumId("Ptr");    //MPointer.
const MType MType_MString  = MEnumId("Str");    //MString.
const MType MType_MLambda  = MEnumId("Lmd");    //MLamdba.
const MType MType_MData    = MEnumId("Dat");    //MData.
const MType MType_MArray   = MEnumId("Arr");    //MArray.
const MType MType_MImage   = MEnumId("Img");    //MImage.
const MType MType_MSpecial = MEnumId("Spc");    //MSpecial.

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
