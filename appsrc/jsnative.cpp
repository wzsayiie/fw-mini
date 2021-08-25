#include "jsnative.h"
#include "minikit.h"

static void include() {
    MString *target = MJsParamString(0);

    if (!target) {
        return;
    }

    MJsRunScriptNamed(target);
}

static void PrintMessage() {
    MString *message = MJsParamString(0);

    if (!message) {
        return;
    }

    MPrintMessage(message);
}

static void RELEASE() {
    MObject *object = MJsParamObject(0);

    if (!object) {
        return;
    }

    MRelease(object);
}

void JSRegisterNativeFuncs() {

    #define REGISTER(n, f) MJsRegisterFunc(n, (m_cast_lambda f).get())

    REGISTER("include"      , include     );
    REGISTER("MPrintMessage", PrintMessage);
    REGISTER("MRelease"     , RELEASE     );
}
