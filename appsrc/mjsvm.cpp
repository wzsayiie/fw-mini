#include "mjsvm.h"

MJsVM::ptr MJsVM::sInstance;

define_reflectable_class_function(MJsVM, setInstance, "args:obj")
void MJsVM::setInstance(const MJsVM::ptr &obj) {
    sInstance = obj;
}

define_reflectable_class_function(MJsVM, instance, "getter")
MJsVM *MJsVM::instance() {
    if (!sInstance) {
        sInstance = MJsVM::create();
    }
    return sInstance.get();
}

define_reflectable_class_function(MJsVM, setExceptionListener, "args:listener")
void MJsVM::setExceptionListener(const MBaseFunction::ptr &listener) {
    implement_injectable_function(void, listener)
}

define_reflectable_class_function(MJsVM, registerFunction, "args:name,func")
void MJsVM::registerFunction(const std::string &name, const MBaseFunction::ptr &func) {
    implement_injectable_function(void, name, func)
}

define_reflectable_class_function(MJsVM, evaluate, "args:name,script")
void MJsVM::evaluate(const std::string &name, const std::string script) {
    implement_injectable_function(void, name, script)
}
