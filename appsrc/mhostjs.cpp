#include "mhostjs.h"
#include <cstring>
#include "mresource.h"

static _MJsRegisterFunc sRegisterFunc = nullptr;
static _MJsRunScript    sRunScript    = nullptr;

void _MJsSetRegisterFunc(_MJsRegisterFunc func) { sRegisterFunc = func; }
void _MJsSetRunScript   (_MJsRunScript    func) { sRunScript    = func; }

static void RegisterFunc(MString *name) {
    if (sRegisterFunc) {
        sRegisterFunc(name);
    } else {
        D("ERROR: no api 'MJsRegisterFunc'");
    }
}

static void RunScript(MString *name, MString *script) {
    if (sRunScript) {
        sRunScript(name, script);
    } else {
        D("ERROR: no api 'MJsRunScript'");
    }
}

struct CallingFrame {

    std::string funcName;
    MArrayRef   params  ;
    MObjectRef  returned;

    CallingFrame(const char *funcName, MArray *params) {
        this->funcName = funcName;
        this->params   = m_make_shared params;
    }
};

m_static_object(sFuncMap      (), std::map<std::string, MLambdaRef>)
m_static_object(sCallingFrames(), std::vector<CallingFrame>)
m_static_object(sErrorListener(), MLambdaRef)
m_static_object(sLastError    (), MStringRef)

MObject *_MJsOnCallCopyRet(MString *name, MArray *params) {
    const char *chars = MStringU8Chars(name);
    if (!chars) {
        return nullptr;
    }

    auto iterator = sFuncMap().find(chars);
    if (iterator == sFuncMap().end()) {
        return nullptr;
    }

    //NOTE: use calling stack. calling may be multi-level.
    sCallingFrames().push_back(CallingFrame(chars, params));
    MLambdaCall(iterator->second.get());

    MObjectRef returnObject = sCallingFrames().back().returned;
    sCallingFrames().pop_back();

    return MRetain(returnObject.get());
}

void _MJsOnHappenError(MString *info) {
    sLastError() = m_make_shared info;
    MLambdaCall(sErrorListener().get());
}

void MJsSetErrorListener(MLambda *listener) {
    sErrorListener() = m_make_shared listener;
}

MString *MJsLastError() {
    return sLastError().get();
}

void MJsRegisterFunc(const char *name, MLambda *func) {
    if (!name || !func) {
        return;
    }

    MStringRef funcName = m_auto_release MStringCreateU8(name);
    RegisterFunc(funcName.get());
    sFuncMap()[name] = m_make_shared func;
}

const char *MJsCallingFuncName() {
    return sCallingFrames().back().funcName.c_str();
}

MArray *MJsCallingParams() {
    return sCallingFrames().back().params.get();
}

void MJsCallingReturn(MObject *value) {
    sCallingFrames().back().returned = m_make_shared value;
}

void MJsRunScript(MString *name, MString *script) {
    RunScript(name, script);
}

void MJsRunFile(MString *name) {
    const char *chars = MStringU8Chars(name);
    if (!chars) {
        return;
    }

    //do not run repeatedly.
    static std::set<std::string> *ranSet = nullptr;
    if (!ranSet) {
        ranSet = new std::set<std::string>;
    }
    
    if (ranSet->find(chars) != ranSet->end()) {
        return;
    }
    ranSet->insert(chars);

    //if the target is a absolute path, load the corresponding file.
    MStringRef script;
    if (/* "x:\x" */ (strlen(chars) >= 4 && chars[1] == ':') ||
        /* "/x"   */ (strlen(chars) >= 2 && chars[0] == '/') )
    {
        script = m_auto_release MCopyStringFromFile(name);
    }
    else
    {
        script = m_auto_release MCopyStringFromBundle(name);
    }
    
    if (!script) {
        D("ERROR: no javascript script named '%s'", chars);
        return;
    }

    RunScript(name, script.get());
}
