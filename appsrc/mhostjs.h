#pragma once

#include "mbasics.h"

typedef void (*_MJsRegisterFunc )(MString *name);
typedef void (*_MJsAsyncDoScript)(MString *name, MString *script, MLambda *complete);

M_FUNC_HOST void _MJsSetRegisterFunc (_MJsRegisterFunc  func);
M_FUNC_HOST void _MJsSetAsyncDoScript(_MJsAsyncDoScript func);

M_FUNC_HOST MObject *_MJsOnCallCopyRet(MString *name, MArray *params);
M_FUNC_HOST void     _MJsOnHappenError(MString *info);

//set error listener.
M_FUNC_EXPORT void MJsSetErrorListener(MLambda *listener) M_META(MJsSetErrorListener);
M_FUNC_EXPORT MString *MJsLastError() M_META(MJsLastError);

//register native function.
M_FUNC_EXPORT void        MJsRegisterFunc   (const char *name, MLambda *func) M_META(MJsRegisterFunc   );
M_FUNC_EXPORT const char *MJsCallingFuncName()                                M_META(MJsCallingFuncName);
M_FUNC_EXPORT MArray     *MJsCallingParams  ()                                M_META(MJsCallingParams  );
M_FUNC_EXPORT void        MJsCallingReturn  (MObject *value)                  M_META(MJsCallingReturn  );

//execute javascript string.
M_FUNC_EXPORT void MJsAsyncDoScript(MString *name, MString *script, MLambda *complete) M_META(MJsAsyncDoScript);
M_FUNC_EXPORT void MJsAsyncDoFile  (MString *name, MLambda *complete)                  M_META(MJsAsyncDoFile  );
