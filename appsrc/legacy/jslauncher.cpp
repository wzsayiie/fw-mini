#include "minikit.h"

static void InstallErrorListener(void (*listener)(MString *)) {
    MLambdaRef wrapper = m_cast_lambda [=]() {
        MString *error = MJsLastError();
        listener(error);
    };
    MJsSetErrorListener(wrapper.get());
}

static std::string EscapeString(MString *string) {
    std::string escaped;
    for (const char *ch = MStringU8Chars(string); *ch; ++ch) {
        switch (*ch)  {
        case '\\': escaped.append("\\\\"); break;
        case '\'': escaped.append("\\\'"); break;
        case '\"': escaped.append("\\\""); break;
        default  : escaped.push_back(*ch);
        }
    }
    return escaped;
}

static void RegisterConsts() {
    std::string script;

    for (MConstSelectFirst(); MConstSelectedValid(); MConstSelectNext()) {
        MTypeId  constType = MConstSelectedTypeId();
        MString *constName = MConstSelectedName();

        const char *name = MStringU8Chars(constName);
        const char *desc = nullptr;
        if (constType == MTypeIdOf<MString *>::Value) {
            auto raw = MConstSelectedString();
            auto str = EscapeString(raw);
            desc = MFormat("const %s ='%s'\n", name, str.c_str());

        } else if (constType == MTypeIdOf<float>::Value) {
            auto flt = MConstSelectedFloat();
            desc = MFormat("const %s = %f \n", name, flt);

        } else if (constType == MTypeIdOf<int>::Value) {
            auto num = MConstSelectedInt();
            desc = MFormat("const %s = %d \n", name, num);
        }

        if (desc) {
            script.append(desc);
        }
    }

    MStringRef runName = m_auto_release MStringCreateU8("builtin_consts");
    MStringRef runCode = m_auto_release MStringCreateU8(script.c_str());
    MJsRunScript(runName.get(), runCode.get());
}

static void NativeFunc() {
    MString *name = MJsCallingFuncName();
    MArray  *args = MJsCallingParams();

    MObjectRef returned = m_auto_release MFuncCallCopyRet(name, args);

    MJsCallingReturn(returned.get());
}

static void RegisterFuncs() {
    MLambdaRef body = m_cast_lambda NativeFunc;
    for (MFuncSelectFirst(); MFuncSelectedValid(); MFuncSelectNext()) {
        MString *name = MFuncSelectedName();
        MJsRegisterFunc(name, body.get());
    }
}

static void ExecuteFile(const char *name) {
    MStringRef file = m_auto_release MStringCreateU8(name);
    MStringRef code = m_auto_release MCopyStringFromBundle(file.get());

    if (code) {
        MJsRunScript(file.get(), code.get());
    }
}

static void OnError(MString *error) {
    MPrintMessage(error);
}

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_Scene) {
    InstallErrorListener(OnError);

    RegisterConsts();
    RegisterFuncs();
    
    ExecuteFile("bundle.js");
}
