#pragma once

#include "mbasics.h"

typedef void (*_MJsRegisterFunc)(MString *name);
typedef void (*_MJsRunScript   )(MString *name, MString *script);

M_FUNC_HOST void _MJsSetRegisterFunc(_MJsRegisterFunc func);
M_FUNC_HOST void _MJsSetRunScript   (_MJsRunScript    func);

M_FUNC_HOST MObject *_MJsOnCallCopyRet(MString *name, MArray *params);
M_FUNC_HOST void     _MJsOnHappenError(MString *info);

//set error listener.
M_FUNC_EXPORT void MJsSetErrorListener(MLambda *listener) M_META(MJsSetErrorListener, "args:listener");
M_FUNC_EXPORT MString *MJsLastError() M_META(MJsLastError);

//register native function.
M_FUNC_EXPORT void     MJsRegisterFunc   (MString *name, MLambda *func) M_META(MJsRegisterFunc   , "args:name,func");
M_FUNC_EXPORT MString *MJsCallingFuncName()                             M_META(MJsCallingFuncName);
M_FUNC_EXPORT MArray  *MJsCallingParams  ()                             M_META(MJsCallingParams  );
M_FUNC_EXPORT void     MJsCallingReturn  (MObject *value)               M_META(MJsCallingReturn  , "args:name,value");

//execute javascript string.
M_FUNC_EXPORT void MJsRunScript(MString *name, MString *script) M_META(MJsRunScript, "args:name,script");
M_FUNC_EXPORT void MJsRunFile  (MString *name)                  M_META(MJsRunFile  , "args:name");
