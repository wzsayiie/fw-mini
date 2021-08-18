#pragma once

#include "mtypes.h"

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

//traverse functions.
MFUNC_EXPORT void MFuncsIteratorBegin();
MFUNC_EXPORT bool MFuncsIteratorValid();
MFUNC_EXPORT void MFuncsIteratorNext ();

//select a function.
MFUNC_EXPORT bool MFuncSelect(const char *name);

//the meta information of selected function:
MFUNC_EXPORT const char *MFuncName();

MFUNC_EXPORT MType MFuncRetType  ();
MFUNC_EXPORT bool  MFuncRetRetain();
MFUNC_EXPORT int   MFuncArgCount ();
MFUNC_EXPORT MType MFuncArgType0 ();
MFUNC_EXPORT MType MFuncArgType1 ();
MFUNC_EXPORT MType MFuncArgType2 ();
MFUNC_EXPORT MType MFuncArgType3 ();

//call function.
MFUNC_EXPORT MObject *MFuncCallCopy(const char *name, MObject *a0, MObject *a1, MObject *a2);
