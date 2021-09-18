#pragma once

#include "mbasics.h"

typedef void (*_MJsRegisterFunc)(MString *name);
typedef void (*_MJsRunScript   )(MString *name, MString *script);

M_FUNC_HOST void _MJsSetRegisterFunc(_MJsRegisterFunc func);
M_FUNC_HOST void _MJsSetRunScript   (_MJsRunScript    func);

M_FUNC_HOST MObject *_MJsOnCallCopyRet(MString *name, MArray *params);
M_FUNC_HOST void     _MJsOnHappenError(MString *info);

//set error listener.
M_FUNC_EXPORT void MJsSetErrorListener(MLambda *listener) M_META(MJsSetErrorListener);
M_FUNC_EXPORT MString *MJsLastError() M_META(MJsLastError);

//register constant.
M_FUNC_EXPORT void MJsRegisterString(const char *name, MString *value) M_META(MJsRegisterString);
M_FUNC_EXPORT void MJsRegisterInt   (const char *name, int      value) M_META(MJsRegisterInt   );
M_FUNC_EXPORT void MJsRegisterFloat (const char *name, float    value) M_META(MJsRegisterFloat );

//register native function.
M_FUNC_EXPORT void        MJsRegisterFunc   (const char *name, MLambda *func) M_META(MJsRegisterFunc   );
M_FUNC_EXPORT const char *MJsCallingFuncName()                                M_META(MJsCallingFuncName);
M_FUNC_EXPORT MArray     *MJsCallingParams  ()                                M_META(MJsCallingParams  );
M_FUNC_EXPORT void        MJsCallingReturn  (MObject *value)                  M_META(MJsCallingReturn  );

//execute javascript string.
M_FUNC_EXPORT void MJsRunScript(MString *name, MString *script) M_META(MJsRunScript);
M_FUNC_EXPORT void MJsRunScriptNamed(MString *name) M_META(MJsRunScriptNamed);
