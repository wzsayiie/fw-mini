#include "mjsvm.h"

MJsVM::ptr MJsVM::sIntance;

define_reflectable_class_function(MJsVM, setInstance, "setter;args:obj")
void MJsVM::setInstance(const MJsVM::ptr &obj) {
    sIntance = obj;
}

define_reflectable_class_function(MJsVM, instance, "getter")
MJsVM *MJsVM::instance() {
    if (!sIntance) {
        sIntance = MJsVM::create();
    }
    return sIntance.get();
}

define_reflectable_class_function(MJsVM, setExceptionListener, "setter;args:listener")
void MJsVM::setExceptionListener(const MBaseFunction::ptr &listener) {
    mExceptionListener = listener;
}

define_reflectable_class_function(MJsVM, exceptionListener, "getter")
MBaseFunction::ptr MJsVM::exceptionListener() {
    return mExceptionListener;
}

define_reflectable_class_function(MJsVM, registerFunction, "args:name,func")
void MJsVM::registerFunction(const std::string &name, const MBaseFunction::ptr &func) {
    if (!name.empty() && func) {
        onRegisterFunction(name, func);
    }
}

define_reflectable_class_function(MJsVM, evaluate, "args:name,script")
void MJsVM::evaluate(const std::string &name, const std::string &script) {
    if (!name.empty() && !script.empty()) {
        onEvaluate(name, script);
    }
}

void MJsVM::onException(const std::string &message) {
    if (mExceptionListener) {
        mExceptionListener->call_with_args({ message });
    }
}

void MJsVM::onRegisterFunction(const std::string &name, const MBaseFunction::ptr &func) {
}

void MJsVM::onEvaluate(const std::string &name, const std::string &script) {
}
