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

extern "C" void  MFuncSelect   (const char *name);
extern "C" MType MFuncRetType  ();
extern "C" bool  MFuncRetRetain();
extern "C" int   MFuncArgCount ();
extern "C" MType MFuncArgType0 ();
extern "C" MType MFuncArgType1 ();
extern "C" MType MFuncArgType2 ();

extern "C" MObject *NFuncCopyCall(const char *name, MObject *a0, MObject *a1, MObject *a2);
