#pragma once

#include "mtypes.h"

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

//traverse functions.
MFUNC_EXPORT void MFuncSelectFirst  () MFUNC_META(MFuncSelectFirst  );
MFUNC_EXPORT bool MFuncSelectedValid() MFUNC_META(MFuncSelectedValid);
MFUNC_EXPORT void MFuncSelectNext   () MFUNC_META(MFuncSelectNext   );

//select a function.
MFUNC_EXPORT bool MFuncSelect(const char *name) MFUNC_META(MFuncSelect);

//the meta information of selected function:
MFUNC_EXPORT const char *MFuncSelectedName() MFUNC_META(MFuncSelectedName);

MFUNC_EXPORT MType MFuncSelectedRetType  ()          MFUNC_META(MFuncSelectedRetType  );
MFUNC_EXPORT bool  MFuncSelectedRetRetain()          MFUNC_META(MFuncSelectedRetRetain);
MFUNC_EXPORT int   MFuncSelectedArgCount ()          MFUNC_META(MFuncSelectedArgCount );
MFUNC_EXPORT MType MFuncSelectedArgType  (int index) MFUNC_META(MFuncSelectedArgType  );

//call function.
MFUNC_EXPORT MObject *MFuncCallCopyRet(const char *name, MArray *args) MFUNC_META(MFuncCallCopyRet);
