#include "minikit.h"

static void SetErrorListener() {
    MLambdaRef listener = m_cast_lambda []() {
        MString *error = MJsLastError();
        MPrintMessage(error);
    };
    MJsSetErrorListener(listener.get());
}

static void RegisterNativeConsts() {
    for (MConstSelectFirst(); MConstSelectedValid(); MConstSelectNext()) {
        const char *name = MConstSelectedName();

        MTypeId typeId = MConstSelectedTypeId();
        switch (typeId) {
            case MTypeIdOf<MString *>::Value: MJsRegisterString(name, MConstSelectedString()); break;
            case MTypeIdOf<int      >::Value: MJsRegisterInt   (name, MConstSelectedInt   ()); break;
            case MTypeIdOf<float    >::Value: MJsRegisterFloat (name, MConstSelectedFloat ()); break;
            default:;
        }
    }
}

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

static void MJsRunFile() {
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

static void MJsLambdaWrap() {
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

static void RegisterBuiltinFuncs() {

    MJsRegisterFunc("MJsRunFile"   , (m_cast_lambda MJsRunFile   ).get());
    MJsRegisterFunc("MJsLambdaWrap", (m_cast_lambda MJsLambdaWrap).get());
}

static void InstallSimulatedNodeJsEnvironment() {
    const char *script =
    "const module = {}                  //01.\n"
    "                                   //02.\n"
    "function require(name) {           //03.\n"
    "   if (!name.endsWith('.js')) {    //04.\n"
    "       name = `${name}.js`         //05.\n"
    "   }                               //06.\n"
    "   MJsRunFile(name)                //07.\n"
    "                                   //07.\n"
    "   return module.exports           //09.\n"
    "}                                  //10.\n";

    MStringRef name = m_auto_release MStringCreateU8("simulated-nodejs");
    MStringRef code = m_auto_release MStringCreateU8(script);
    MJsRunScript(name.get(), code.get());
}

static void LaunchEntryFile(const char *file, const char *func) {
    MStringRef launchFile = m_auto_release MStringCreateU8(file);
    MJsRunScriptNamed(launchFile.get());

    MStringRef launchFunc = m_auto_release MStringCreateU8(func);
    MJsRunScript(launchFunc.get(), launchFunc.get());
}

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_Scene) {
    SetErrorListener();

    RegisterNativeConsts();
    RegisterNativeFuncs ();
    RegisterBuiltinFuncs();

    InstallSimulatedNodeJsEnvironment();

    LaunchEntryFile("app.js", "Launch()");
}
