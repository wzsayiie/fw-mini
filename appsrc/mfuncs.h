#pragma once

#include "mtypes.h"

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

//traverse functions.
MFUNC_EXPORT void MFuncSelectFirst  ();
MFUNC_EXPORT bool MFuncSelectedValid();
MFUNC_EXPORT void MFuncSelectNext   ();

//select a function.
MFUNC_EXPORT bool MFuncSelect(const char *name);

//the meta information of selected function:
MFUNC_EXPORT const char *MFuncSelectedName();

MFUNC_EXPORT MType MFuncSelectedRetType  ();
MFUNC_EXPORT bool  MFuncSelectedRetRetain();
MFUNC_EXPORT int   MFuncSelectedArgCount ();
MFUNC_EXPORT MType MFuncSelectedArgType  (int index);

//call function.
MFUNC_EXPORT MObject *MFuncCallCopyRet(const char *name, MArray *args);
