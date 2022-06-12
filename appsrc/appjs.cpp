#include "minikit.h"

static void onException(const std::string &message) {
    MPrint(message);
}

static void registerNativeFunctions() {
}

static void registerNativeMetas() {
}

static void registerBuiltinFunctions() {
}

static void registerBuiltinScript() {
}

static void launch() m_app_launch(launch) {
    MJsVM::instance()->setExceptionListener(MF(onException));
    
    registerNativeFunctions();
    registerNativeMetas();
    registerBuiltinFunctions();
    registerBuiltinScript();

    std::string name = "bundle.js";
    std::string code = MFileManager::instance()->u8stringFromBundle(name);
    MJsVM::instance()->evaluate(name, code);
}
