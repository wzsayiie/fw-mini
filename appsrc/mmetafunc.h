#pragma once

#include "mbasics.h"

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

//traverse functions.
M_FUNC_EXPORT void MFuncSelectFirst  () MFUNC_META(MFuncSelectFirst  );
M_FUNC_EXPORT bool MFuncSelectedValid() MFUNC_META(MFuncSelectedValid);
M_FUNC_EXPORT void MFuncSelectNext   () MFUNC_META(MFuncSelectNext   );

//select a function.
M_FUNC_EXPORT bool MFuncSelect(const char *name) MFUNC_META(MFuncSelect);

//the meta information of selected function:
M_FUNC_EXPORT const char *MFuncSelectedName() MFUNC_META(MFuncSelectedName);

M_FUNC_EXPORT MTypeId MFuncSelectedRetTypeId()          MFUNC_META(MFuncSelectedRetTypeId);
M_FUNC_EXPORT bool    MFuncSelectedRetRetain()          MFUNC_META(MFuncSelectedRetRetain);
M_FUNC_EXPORT int     MFuncSelectedArgCount ()          MFUNC_META(MFuncSelectedArgCount );
M_FUNC_EXPORT MTypeId MFuncSelectedArgTypeId(int index) MFUNC_META(MFuncSelectedArgTypeId);

//call function.
M_FUNC_EXPORT MObject *MFuncCallCopyRet(const char *name, MArray *args) MFUNC_META(MFuncCallCopyRet);
