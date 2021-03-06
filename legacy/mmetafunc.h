#pragma once

#include "mbasics.h"

void _MFuncSetMeta(const char *name, _MFuncMeta meta, const char *note);

//traverse functions.
M_FUNC_EXPORT void MFuncSelectFirst  () M_META(MFuncSelectFirst  );
M_FUNC_EXPORT bool MFuncSelectedValid() M_META(MFuncSelectedValid);
M_FUNC_EXPORT void MFuncSelectNext   () M_META(MFuncSelectNext   );

//select a function.
M_FUNC_EXPORT bool MFuncSelect(const char *name) M_META(MFuncSelect, "args:name");

//the meta information of selected function:
M_FUNC_EXPORT MString *MFuncSelectedName     () M_META(MFuncSelectedName     );
M_FUNC_EXPORT MString *MFuncSelectedNote     () M_META(MFuncSelectedNote     );
M_FUNC_EXPORT MTypeId  MFuncSelectedRetTypeId() M_META(MFuncSelectedRetTypeId);
M_FUNC_EXPORT bool     MFuncSelectedRetRetain() M_META(MFuncSelectedRetRetain);
M_FUNC_EXPORT int      MFuncSelectedArgCount () M_META(MFuncSelectedArgCount );

M_FUNC_EXPORT MTypeId  MFuncSelectedArgTypeId(int index) M_META(MFuncSelectedArgTypeId, "args:index");

//call function.
M_FUNC_EXPORT MObject *MFuncCallCopyRet(MString *name, MArray *args) M_META(MFuncCallCopyRet, "args:name,args");
