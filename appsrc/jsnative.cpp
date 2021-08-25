#define _CRT_SECURE_NO_WARNINGS

#include "jsnative.h"
#include <cstdio>
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

static void Release() {
    MObject *object = MJsParamObject(0);

    if (!object) {
        return;
    }

    MRelease(object);
}

static void GetType() {
    MObject *object = MJsParamObject(0);

    if (!object) {
        return;
    }

    MType type = MGetType(object);
    MJsReturnInt(type);
}

class JsLambda : public MSpecial {

public:
    JsLambda(int id) {
        mId = id;
    }

    static void call(MObject *load) {
        auto lambda = (JsLambda *)load;
        lambda->run("inner.callLambda(%d)");
    }

    ~JsLambda() {
        run("inner.removeLambda(%d)");
    }

private:
    void run(const char *format) {
        char string[64] = "\0";
        sprintf(string, format, mId);

        MStringRef script = m_auto_release MStringCreateU8(string);
        MJsRunScript(script.get(), script.get());
    }

    int mId;
};

static void LambdaCreate() {
    int id = MJsParamInt(0);

    if (id == 0) {
        return;
    }

    MObjectRef actual = m_auto_release new JsLambda(id);
    MLambdaRef lambda = m_auto_release MLambdaCreate(JsLambda::call, actual.get());
    MJsReturnObject(lambda.get());
}

static void LambdaCall() {
    MLambda *lambda = MJsParamLambda(0);

    if (!lambda) {
        return;
    }

    MLambdaCall(lambda);
}

static void ArrayCreate() {
    MArrayRef array = m_auto_release MArrayCreate();
    MJsReturnObject(array.get());
}

static void ArrayAppend() {
    MArray  *array = MJsParamArray (0);
    MObject *item  = MJsParamObject(1);

    if (!array) {
        return;
    }

    MArrayAppend(array, item);
}

static void ArrayLength() {
    MArray *array = MJsParamArray(0);

    if (!array) {
        return;
    }

    int length = MArrayLength(array);
    MJsReturnInt(length);
}

static void ArrayItem() {
    MArray *array = MJsParamArray(0);
    int     index = MJsParamInt  (1);

    if (!array) {
        return;
    }

    MObject *item = MArrayItem(array, index);
    MJsReturnObject(item);
}

void JSRegisterNativeFuncs() {

    #define REGISTER(n, f) MJsRegisterFunc(n, (m_cast_lambda f).get())

    REGISTER("include"      , include     );
    REGISTER("MPrintMessage", PrintMessage);
    REGISTER("MRelease"     , Release     );
    REGISTER("MGetType"     , GetType     );
    REGISTER("MLambdaCreate", LambdaCreate);
    REGISTER("MLambdaCall"  , LambdaCall  );
    REGISTER("MArrayCreate" , ArrayCreate );
    REGISTER("MArrayAppend" , ArrayAppend );
    REGISTER("MArrayLength" , ArrayLength );
    REGISTER("MArrayItem"   , ArrayItem   );
}
