#include "mjsvm.h"

//js vm:

MJsVM *MJsVM::instance() {
    static auto obj = MJsVM::create();
    return obj.get();
}

void MJsVM::setExceptionListener(const MBaseFunction::ptr &listener) {
    MVirtualJsVM::instance()->setExceptionListener(listener);
}

void MJsVM::registerFunction(const std::string &name, const MBaseFunction::ptr &func) {
    if (!name.empty() && func) {
        MVirtualJsVM::instance()->registerFunction(name, func);
    }
}

void MJsVM::evaluate(const std::string &name, const std::string &script) {
    if (!name.empty() && !script.empty()) {
        MVirtualJsVM::instance()->evaluate(name, script);
    }
}

//virtual js vm:

MVirtualJsVM::ptr MVirtualJsVM::sInstance;

void MVirtualJsVM::setInstance(const MVirtualJsVM::ptr &obj) {
    sInstance = obj;
}

MVirtualJsVM *MVirtualJsVM::instance() {
    if (!sInstance) {
        sInstance = MVirtualJsVM::create();
    }
    return sInstance.get();
}

void MVirtualJsVM::setExceptionListener(const MBaseFunction::ptr &listener) {
}

void MVirtualJsVM::registerFunction(const std::string &name, const MBaseFunction::ptr &func) {
}

void MVirtualJsVM::evaluate(const std::string &name, const std::string &script) {
}
