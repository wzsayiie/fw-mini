#pragma once

#include "mbasics.h"

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

//traverse functions.
MFUNC_EXPORT void MFuncSelectFirst  () MFUNC_META(MFuncSelectFirst  );
MFUNC_EXPORT bool MFuncSelectedValid() MFUNC_META(MFuncSelectedValid);
MFUNC_EXPORT void MFuncSelectNext   () MFUNC_META(MFuncSelectNext   );

//select a function.
MFUNC_EXPORT bool MFuncSelect(const char *name) MFUNC_META(MFuncSelect);

//the meta information of selected function:
MFUNC_EXPORT const char *MFuncSelectedName() MFUNC_META(MFuncSelectedName);

MFUNC_EXPORT MTypeId MFuncSelectedRetTypeId()          MFUNC_META(MFuncSelectedRetTypeId);
MFUNC_EXPORT bool    MFuncSelectedRetRetain()          MFUNC_META(MFuncSelectedRetRetain);
MFUNC_EXPORT int     MFuncSelectedArgCount ()          MFUNC_META(MFuncSelectedArgCount );
MFUNC_EXPORT MTypeId MFuncSelectedArgTypeId(int index) MFUNC_META(MFuncSelectedArgTypeId);

//call function.
MFUNC_EXPORT MObject *MFuncCallCopyRet(const char *name, MArray *args) MFUNC_META(MFuncCallCopyRet);
