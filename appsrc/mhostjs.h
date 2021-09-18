#pragma once

#include "mbasics.h"

typedef void (*_MJsRegisterFunc)(MString *name);
typedef void (*_MJsRunScript   )(MString *name, MString *script);

M_FUNC_HOST void _MJsSetRegisterFunc(_MJsRegisterFunc func);
M_FUNC_HOST void _MJsSetRunScript   (_MJsRunScript    func);

M_FUNC_HOST MObject *_MJsOnCallCopyRet(MString *name, MArray *params);
M_FUNC_HOST void     _MJsOnHappenError(MString *info);

//set error listener.
M_FUNC_EXPORT void MJsSetErrorListener(MLambda *listener) MFUNC_META(MJsSetErrorListener);
M_FUNC_EXPORT MString *MJsLastError() MFUNC_META(MJsLastError);

//register native function:
M_FUNC_EXPORT void        MJsRegisterFunc   (const char *name, MLambda *func) MFUNC_META(MJsRegisterFunc   );
M_FUNC_EXPORT const char *MJsCallingFuncName()                                MFUNC_META(MJsCallingFuncName);
M_FUNC_EXPORT MArray     *MJsCallingParams  ()                                MFUNC_META(MJsCallingParams  );
M_FUNC_EXPORT void        MJsCallingReturn  (MObject *value)                  MFUNC_META(MJsCallingReturn  );

//execute javascript string.
M_FUNC_EXPORT void MJsRunScript(MString *name, MString *script) MFUNC_META(MJsRunScript);
M_FUNC_EXPORT void MJsRunScriptNamed(MString *name) MFUNC_META(MJsRunScriptNamed);
