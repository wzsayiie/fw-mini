#include "mhostjs.h"
#include <cstring>
#include <set>
#include "mresource.h"

struct CallingFrame {

    std::string funcName;
    MArrayRef   params  ;
    MObjectRef  returned;

    CallingFrame(const char *funcName, MArray *params) {
        this->funcName = funcName;
        this->params   = m_make_shared params;
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
    sCallingFrames.push_back(CallingFrame(chars, params));
    MLambdaCall(iterator->second.get());

    MObjectRef returnObject = sCallingFrames.back().returned;
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

const char *MJsCallingFuncName() {
    return sCallingFrames.back().funcName.c_str();
}

MArray *MJsCallingParams() {
    return sCallingFrames.back().params.get();
}

void MJsCallingReturn(MObject *value) {
    sCallingFrames.back().returned = m_make_shared value;
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
