#include "mhostjs.h"

struct CallingFrame {

    MArrayRef  params;
    MObjectRef retValue;

    CallingFrame(MArray *params) {
        this->params = m_make_shared params;
    }
};

static _MJsRegisterFunc sRegisterFunc = nullptr;
static _MJsRunScript    sRunScript    = nullptr;

static std::map<std::string, MLambdaRef> sFuncMap;
static std::vector<CallingFrame> sCallingFrames;
static MLambdaRef sErrorListener;
static MStringRef sLastError;

void _MJsSetRegisterFunc(_MJsRegisterFunc func) { sRegisterFunc = func; }
void _MjsSetRunScript   (_MJsRunScript    func) { sRunScript    = func; }

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

    MObject *coptRet = MRetain(sCallingFrames.back().retValue.get());
    sCallingFrames.pop_back();

    return coptRet;
}

void _MJsOnHappenError(MString *info) {
    sLastError = m_make_shared info;
    MLambdaCall(sErrorListener.get());
}

void MJsSetErrorListener(MLambda *listener) {
    sErrorListener = m_make_shared listener;
}

MString *MJsGetLastError() {
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

MArray *MJsCallingParams() {
    return sCallingFrames.back().params.get();
}

void MJsCallingReturn(MObject *value) {
    sCallingFrames.back().retValue = m_make_shared value;
}

void MJsRunScript(MString *name, MString *script) {
    if (sRunScript && script) {
        sRunScript(name, script);
    }
}
