#include "minikit.h"

static void SetErrorListener() {
    MLambdaRef listener = m_cast_lambda []() {
        MString *error = MJsLastError();
        MPrintMessage(error);
    };
    MJsSetErrorListener(listener.get());
}

static std::string EscapedString(MString *string) {
    std::string escaped;
    for (const char *ch = MStringU8Chars(string); *ch; ++ch) {
        switch (*ch)  {
        case '\\': escaped.append("\\\\"); break;
        case '\'': escaped.append("\\\'"); break;
        case '"' : escaped.append("\\\""); break;
        default  : escaped.push_back(*ch);
        }
    }
    return escaped;
}

static void RegisterNativeConsts() {
    std::string script;

    for (MConstSelectFirst(); MConstSelectedValid(); MConstSelectNext()) {
        const char *name = MConstSelectedName();
        const char *desc = nullptr;

        MTypeId typeId = MConstSelectedTypeId();
        if (typeId == MTypeIdOf<MString *>::Value) {
            MString *value = MConstSelectedString();
            std::string escaped = EscapedString(value);
            desc = MFormat("const %s = '%s'\n" , name, escaped.c_str());

        } else if (typeId == MTypeIdOf<float>::Value) {
            float value = MConstSelectedFloat();
            desc = MFormat("const %s = %f\n", name, value);

        } else if (typeId == MTypeIdOf<int>::Value) {
            int value = MConstSelectedInt();
            desc = MFormat("const %s = %d\n", name, value);
        }

        if (desc) {
            script.append(desc);
        }
    }

    MStringRef name = m_auto_release MStringCreateU8("builtin_consts");
    MStringRef code = m_auto_release MStringCreateU8(script.c_str());
    MJsRunScript(name.get(), code.get());
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

class JsLambdaActual : public MUnknown {

public:
    JsLambdaActual(int iden) {
        mIden = iden;
    }

    static void call(MObject *load) {
        auto lambda = (JsLambdaActual *)load;
        lambda->run("MJsLambdaInvoke(%d)");
    }

    ~JsLambdaActual() {
        run("MJsLambdaRemove(%d)");
    }

private:
    void run(const char *format) {
        const char *string = MFormat(format, mIden);
        MStringRef  script = m_auto_release MStringCreateU8(string);

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
    MJsRegisterFunc("MJsLambdaWrap", (m_cast_lambda MJsLambdaWrap).get());
}

static void ExecuteFile(const char *name) {
    MStringRef file = m_auto_release MStringCreateU8(name);
    MJsRunFile(file.get());
}

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_Scene) {
    SetErrorListener();

    RegisterNativeConsts();
    RegisterNativeFuncs ();
    RegisterBuiltinFuncs();
    
    ExecuteFile("runtime/runtime.js");
    ExecuteFile("app.js");
}
