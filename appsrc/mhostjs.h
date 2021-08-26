#pragma once

#include "mtypes.h"

typedef void (*_MJsRegisterFunc)(MString *name);
typedef void (*_MJsRunScript   )(MString *name, MString *script);

MFUNC_HOST void _MJsSetRegisterFunc(_MJsRegisterFunc func);
MFUNC_HOST void _MJsSetRunScript   (_MJsRunScript    func);

MFUNC_HOST MObject *_MJsOnCallCopyRet(MString *name, MArray *params);
MFUNC_HOST void     _MJsOnHappenError(MString *info);

//set error listener.
MFUNC_EXPORT void MJsSetErrorListener(MLambda *listener) MFUNC_META(MJsSetErrorListener);
MFUNC_EXPORT MString *MJsLastError() MFUNC_META(MJsLastError);

//register native function:
MFUNC_EXPORT void        MJsRegisterFunc   (const char *name, MLambda *func) MFUNC_META(MJsRegisterFunc   );
MFUNC_EXPORT const char *MJsCallingFuncName()                                MFUNC_META(MJsCallingFuncName);
MFUNC_EXPORT MArray     *MJsCallingParams  ()                                MFUNC_META(MJsCallingParams  );
MFUNC_EXPORT void        MJsCallingReturn  (MObject *value)                  MFUNC_META(MJsCallingReturn  );

//execute javascript string.
MFUNC_EXPORT void MJsRunScript(MString *name, MString *script) MFUNC_META(MJsRunScript);
MFUNC_EXPORT void MJsRunScriptNamed(MString *name) MFUNC_META(MJsRunScriptNamed);
