#include "jsnative.h"
#include "minikit.h"

static void OnError() {
    MString *error = MJsLastError();
    MPrintMessage(error);
}

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_Scene) {

    MJsSetErrorListener((m_cast_lambda OnError).get());
    JSRegisterNativeFuncs();

    MStringRef launchFile = m_auto_release MStringCreateU8("app.js");
    MJsRunScriptNamed(launchFile.get());

    MStringRef launcFunc = m_auto_release MStringCreateU8("launch()");
    MJsRunScript(launcFunc.get(), launcFunc.get());
}
