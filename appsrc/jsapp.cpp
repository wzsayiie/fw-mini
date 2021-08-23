#include <cstring>
#include <set>
#include "minikit.h"

static MString *GetCallingString(MArray *params, int index) {
    MObject *object = MArrayItem(params, index);

    if (object && MGetType(object) == MType_MString) {
        return (MString *)object;
    }
    return nullptr;
}

static void C_include() {
    MArray  *params = MJsCallingParams();
    MString *target = GetCallingString(params, 0);

    if (!target) {
        return;
    }

    std::string aim = MStringU8Chars(target);

    //do not include repeatedly.
    static std::set<std::string> *includes = nullptr;
    if (!includes) {
        includes = new std::set<std::string>;
    }
    if (includes->find(aim) != includes->end()) {
        return;
    }

    //if the target is a file path, load the corresponding file.
    MStringRef script;
    if (strchr(aim.c_str(), '/') || strchr(aim.c_str(), '\\')) {
        script = m_auto_release MCopyStringFromFile(target);
    } else {
        script = m_auto_release MCopyStringFromBundle(target);
    }

    MJsRunScript(target, script.get());
}

static void C_PrintMessage() {
    MArray  *params  = MJsCallingParams();
    MString *message = GetCallingString(params, 0);

    if (!message) {
        return;
    }

    MPrintMessage(message);
}

static void RegisterFunc(const char *name, void (*func)()) {
    MLambdaRef lambda = m_cast_lambda [func]() { func(); };
    MJsRegisterFunc(name, lambda.get());
}

static void OnError() {
    MString *error = MJsGetLastError();
    MPrintMessage(error);
}

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_Scene) {

    //register native functions.
    RegisterFunc("include"      , C_include     );
    RegisterFunc("MPrintMessage", C_PrintMessage);

    //register error listening.
    MLambdaRef errorListener = m_cast_lambda []() { OnError(); };
    MJsSetErrorListener(errorListener.get());

    //execute entry script file.
    MStringRef scriptFile = m_auto_release MStringCreateU8("app.js");
    MStringRef scriptCode = m_auto_release MCopyStringFromBundle(scriptFile.get());
    MJsRunScript(scriptFile.get(), scriptCode.get());

    MStringRef entryFunc = m_auto_release MStringCreateU8("launch()");
    MJsRunScript(entryFunc.get(), entryFunc.get());
}
