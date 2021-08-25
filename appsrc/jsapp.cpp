#include <cstring>
#include <set>
#include "minikit.h"

static void RunScriptNamed(MString *name) {
    const char *nameChars = MStringU8Chars(name);
    if (!nameChars) {
        return;
    }

    //do not run repeatedly.
    static std::set<std::string> *ranSet = nullptr;
    if (!ranSet) {
        ranSet = new std::set<std::string>;
    }
    if (ranSet->find(nameChars) != ranSet->end()) {
        return;
    }

    //if the target is a file path, load the corresponding file.
    MStringRef script;
    if (strchr(nameChars, '/') || strchr(nameChars, '\\')) {
        script = m_auto_release MCopyStringFromFile(name);
    } else {
        script = m_auto_release MCopyStringFromBundle(name);
    }

    if (script) {
        MJsRunScript(name, script.get());
    }
}

static MObject *GetObject(MArray *params, int index) {
    return MArrayItem(params, index);
}

static MString *GetString(MArray *params, int index) {
    MObject *object = MArrayItem(params, index);

    if (MGetType(object) == MType_MString) {
        return (MString *)object;
    }
    return nullptr;
}

static void C_include() {
    MArray  *params = MJsCallingParams();
    MString *target = GetString(params, 0);

    if (!target) {
        return;
    }

    RunScriptNamed(target);
}

static void C_PrintMessage() {
    MArray  *params  = MJsCallingParams();
    MString *message = GetString(params, 0);

    if (!message) {
        return;
    }

    MPrintMessage(message);
}

static void C_RELEASE() {
    MArray  *params = MJsCallingParams();
    MObject *object = GetObject(params, 0);

    if (!object) {
        return;
    }

    MRelease(object);
}

static void RegisterFunc(const char *name, void (*func)()) {
    MLambdaRef lambda = m_cast_lambda [func]() { func(); };
    MJsRegisterFunc(name, lambda.get());
}

static void InstallErrorListener() {
    MLambdaRef listener = m_cast_lambda []() {

        MString *error = MJsGetLastError();
        MPrintMessage(error);
    };
    MJsSetErrorListener(listener.get());
}

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_Scene) {

    //register native functions.
    RegisterFunc("include"      , C_include     );
    RegisterFunc("MPrintMessage", C_PrintMessage);
    RegisterFunc("MRelease"     , C_RELEASE     );

    //install error listening.
    InstallErrorListener();

    //execute entry script file.
    MStringRef launchFile = m_auto_release MStringCreateU8("app.js");
    RunScriptNamed(launchFile.get());

    MStringRef launcFunc = m_auto_release MStringCreateU8("launch()");
    MJsRunScript(launcFunc.get(), launcFunc.get());
}
