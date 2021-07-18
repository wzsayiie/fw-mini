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

MFUNC_BASE void  MFuncSelect   (const char *name);
MFUNC_BASE MType MFuncRetType  ();
MFUNC_BASE bool  MFuncRetRetain();
MFUNC_BASE int   MFuncArgCount ();
MFUNC_BASE MType MFuncArgType0 ();
MFUNC_BASE MType MFuncArgType1 ();
MFUNC_BASE MType MFuncArgType2 ();

MFUNC_BASE MObject *NFuncCopyCall(const char *name, MObject *a0, MObject *a1, MObject *a2);
