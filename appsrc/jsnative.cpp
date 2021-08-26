#define _CRT_SECURE_NO_WARNINGS

#include "jsnative.h"
#include <cstdio>
#include "minikit.h"

class JsLambdaActual : public MSpecial {

public:
    JsLambdaActual(int iden) {
        mIden = iden;
    }

    static void call(MObject *load) {
        auto lambda = (JsLambdaActual *)load;
        lambda->run("_MJsLambdaInvoke(%d)");
    }

    ~JsLambdaActual() {
        run("_MJsLambdaRemove(%d)");
    }

private:
    void run(const char *format) {
        char string[64] = "\0";
        sprintf(string, format, mIden);

        MStringRef script = m_auto_release MStringCreateU8(string);
        MJsRunScript(script.get(), script.get());
    }

    int mIden;
};

static void N_include() {
    MArray  *params = MJsCallingParams();
    MObject *target = MArrayItem(params, 0);

    if (MGetType(target) == MType_MString) {
        MJsRunScriptNamed((MString *)target);
    }
}

static void N_MJsLambdaCreate() {
    MArray  *params = MJsCallingParams();
    MObject *object = MArrayItem(params, 0);

    if (MGetType(object) != MType_MInt) {
        return;
    }

    int iden = MIntValue((MInt *)object);
    MObjectRef actual = m_auto_release new JsLambdaActual(iden);
    MLambdaRef lambda = m_auto_release MLambdaCreate(JsLambdaActual::call, actual.get());
    MJsCallingReturn(lambda.get());
}

static void N_GeneralFunc() {
    const char *funcName = MJsCallingFuncName();
    MArray     *params   = MJsCallingParams();
    MObjectRef  returned = m_auto_release MFuncCallCopyRet(funcName, params);

    MJsCallingReturn(returned.get());
}

void JSRegisterNativeFuncs() {

    MJsRegisterFunc("include"        , (m_cast_lambda N_include        ).get());
    MJsRegisterFunc("MJsLambdaCreate", (m_cast_lambda N_MJsLambdaCreate).get());

    MLambdaRef general = m_cast_lambda N_GeneralFunc;
    for (MFuncSelectFirst(); MFuncSelectedValid(); MFuncSelectNext()) {
        const char *name = MFuncSelectedName();
        MJsRegisterFunc(name, general.get());
    }
}
