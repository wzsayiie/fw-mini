#pragma once

#include "mbasics.h"

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

//traverse functions.
M_FUNC_EXPORT void MFuncSelectFirst  () M_META(MFuncSelectFirst  );
M_FUNC_EXPORT bool MFuncSelectedValid() M_META(MFuncSelectedValid);
M_FUNC_EXPORT void MFuncSelectNext   () M_META(MFuncSelectNext   );

//select a function.
M_FUNC_EXPORT bool MFuncSelect(const char *name) M_META(MFuncSelect);

//the meta information of selected function:
M_FUNC_EXPORT const char *MFuncSelectedName() M_META(MFuncSelectedName);

M_FUNC_EXPORT MTypeId MFuncSelectedRetTypeId()          M_META(MFuncSelectedRetTypeId);
M_FUNC_EXPORT bool    MFuncSelectedRetRetain()          M_META(MFuncSelectedRetRetain);
M_FUNC_EXPORT int     MFuncSelectedArgCount ()          M_META(MFuncSelectedArgCount );
M_FUNC_EXPORT MTypeId MFuncSelectedArgTypeId(int index) M_META(MFuncSelectedArgTypeId);

//call function.
M_FUNC_EXPORT MObject *MFuncCallCopyRet(const char *name, MArray *args) M_META(MFuncCallCopyRet);
