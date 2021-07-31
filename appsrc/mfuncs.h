#pragma once

#include "mtypes.h"

struct _MFuncInfo {
    void *address   = nullptr;
    MType retType   = 0;
    bool  retRetain = false;
    int   argCount  = 0;
    MType argType0  = 0;
    MType argType1  = 0;
    MType argType2  = 0;
};

void _MFuncAdd(const char *name, const _MFuncInfo &info);

MFUNC_EXPORT void  MFuncSelect   (const char *name);
MFUNC_EXPORT MType MFuncRetType  ();
MFUNC_EXPORT bool  MFuncRetRetain();
MFUNC_EXPORT int   MFuncArgCount ();
MFUNC_EXPORT MType MFuncArgType0 ();
MFUNC_EXPORT MType MFuncArgType1 ();
MFUNC_EXPORT MType MFuncArgType2 ();

MFUNC_EXPORT MObject *NFuncCopyCall(const char *name, MObject *a0, MObject *a1, MObject *a2);
