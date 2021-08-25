#include "mhostjs.h"
#include <cstring>
#include <set>
#include "mresource.h"

struct CallingFrame {

    MArrayRef  callingParams;
    MObjectRef returnObject ;

    CallingFrame(MArray *params) {
        this->callingParams = m_make_shared params;
    }
};

static _MJsRegisterFunc sRegisterFunc = nullptr;
static _MJsRunScript    sRunScript    = nullptr;

static std::map<std::string, MLambdaRef> sFuncMap;
static std::vector<CallingFrame> sCallingFrames;
static MLambdaRef sErrorListener;
static MStringRef sLastError;

void _MJsSetRegisterFunc(_MJsRegisterFunc func) { sRegisterFunc = func; }
void _MJsSetRunScript   (_MJsRunScript    func) { sRunScript    = func; }

MObject *_MJsOnCallCopyRet(MString *name, MArray *params) {
    const char *chars = MStringU8Chars(name);
    if (!chars) {
        return nullptr;
    }

    auto iterator = sFuncMap.find(chars);
    if (iterator == sFuncMap.end()) {
        return nullptr;
    }

    //NOTE: use calling stack. calling may be multi-level.
    sCallingFrames.push_back(CallingFrame(params));
    MLambdaCall(iterator->second.get());

    MObjectRef returnObject = sCallingFrames.back().returnObject;
    sCallingFrames.pop_back();

    return MRetain(returnObject.get());
}

void _MJsOnHappenError(MString *info) {
    sLastError = m_make_shared info;
    MLambdaCall(sErrorListener.get());
}

void MJsSetErrorListener(MLambda *listener) {
    sErrorListener = m_make_shared listener;
}

MString *MJsLastError() {
    return sLastError.get();
}

void MJsRegisterFunc(const char *name, MLambda *func) {
    if (!name || !func) {
        return;
    }

    if (sRegisterFunc) {
        MStringRef func = m_auto_release MStringCreateU8(name);
        sRegisterFunc(func.get());
    }
    sFuncMap[name] = m_make_shared func;
}

int MJsParamInt(int index) {
    MArray  *params = sCallingFrames.back().callingParams.get();
    MObject *object = MArrayItem(params, index);

    switch (MGetType(object)) {
        case MType_MBool : return (int)MBoolValue ((MBool  *)object);
        case MType_MInt  : return (int)MIntValue  ((MInt   *)object);
        case MType_MFloat: return (int)MFloatValue((MFloat *)object);

        default: return 0;
    }
}

MObject *MJsParamObject(int index) {
    MArray *params = sCallingFrames.back().callingParams.get();
    return MArrayItem(params, index);
}

template<typename T, MType ID> T JsParam(int index) {
    MArray  *params = sCallingFrames.back().callingParams.get();
    MObject *object = MArrayItem(params, index);

    if (MGetType(object) == ID) {
        return (T)object;
    }
    return nullptr;
}

MString *MJsParamString(int index) { return JsParam<MString *, MType_MString>(index); }
MLambda *MJsParamLambda(int index) { return JsParam<MLambda *, MType_MLambda>(index); }
MArray  *MJsParamArray (int index) { return JsParam<MArray  *, MType_MArray >(index); }

void MJsReturnInt(int value) {
    sCallingFrames.back().returnObject = m_auto_release MIntCreate(value);
}

void MJsReturnObject(MObject *value) {
    sCallingFrames.back().returnObject = m_make_shared value;
}

void MJsRunScript(MString *name, MString *script) {
    if (sRunScript && script) {
        sRunScript(name, script);
    }
}

void MJsRunScriptNamed(MString *name) {
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

    if (sRunScript && script) {
        sRunScript(name, script.get());
    }
}
