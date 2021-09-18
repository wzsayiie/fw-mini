#include "minikit.h"

static void NativeFunc() {
    const char *funcName = MJsCallingFuncName();
    MArray     *params   = MJsCallingParams();
    MObjectRef  returned = m_auto_release MFuncCallCopyRet(funcName, params);

    MJsCallingReturn(returned.get());
}

static void RegisterNativeFuncs() {
    MLambdaRef body = m_cast_lambda NativeFunc;
    for (MFuncSelectFirst(); MFuncSelectedValid(); MFuncSelectNext()) {
        const char *name = MFuncSelectedName();
        MJsRegisterFunc(name, body.get());
    }
}

static void Include() {
    MArray  *params = MJsCallingParams();
    MObject *target = MArrayItem(params, 0);

    if (MGetTypeId(target) == MTypeIdOf<MString *>::Value) {
        MJsRunScriptNamed((MString *)target);
    }
}

class JsLambdaActual : public MUnknown {

public:
    JsLambdaActual(int iden) {
        mIden = iden;
    }

    static void call(MObject *load) {
        auto lambda = (JsLambdaActual *)load;
        lambda->run("_JsLambdaInvoke(%d)");
    }

    ~JsLambdaActual() {
        run("_JsLambdaRemove(%d)");
    }

private:
    void run(const char *format) {
        const char *express = MFormat(format, mIden);
        MStringRef script = m_auto_release MStringCreateU8(express);
        MJsRunScript(script.get(), script.get());
    }

    int mIden;
};

static void MJsLambdaCreate() {
    MArray  *params = MJsCallingParams();
    MObject *object = MArrayItem(params, 0);

    if (MGetTypeId(object) != MTypeIdOf<MInt *>::Value) {
        return;
    }

    int iden = MIntValue((MInt *)object);
    MObjectRef actual = m_auto_release new JsLambdaActual(iden);
    MLambdaRef lambda = m_auto_release MLambdaCreate(JsLambdaActual::call, actual.get());
    MJsCallingReturn(lambda.get());
}

static void RegisterBuiltFuncs() {

    MJsRegisterFunc("include"        , (m_cast_lambda Include        ).get());
    MJsRegisterFunc("MJsLambdaCreate", (m_cast_lambda MJsLambdaCreate).get());
}

static void SetErrorListener() {
    MLambdaRef listener = m_cast_lambda []() {
        MString *error = MJsLastError();
        MPrintMessage(error);
    };
    MJsSetErrorListener(listener.get());
}

static void LaunchEntryFile(const char *file, const char *func) {
    MStringRef launchFile = m_auto_release MStringCreateU8(file);
    MJsRunScriptNamed(launchFile.get());

    MStringRef launchFunc = m_auto_release MStringCreateU8(func);
    MJsRunScript(launchFunc.get(), launchFunc.get());
}

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_Scene) {
    RegisterNativeFuncs();
    RegisterBuiltFuncs();
    SetErrorListener();

    LaunchEntryFile("app.js", "Launch()");
}
