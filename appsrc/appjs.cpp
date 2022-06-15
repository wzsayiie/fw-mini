#include "minikit.h"

static void MInjectFunction(
    const char *clsName, const char *fcnName, const reflect::base_function::ptr &func)
{
    reflect::inject(clsName, fcnName, func);
}

static void MSetClassSymbol(const reflect::injectable::ptr &obj, const char *symName) {
    if (obj) {
        reflect::symbol sym = reflect::symbol::make(symName);
        obj->set_class_symbol(sym);
    }
}

static std::string MGetOS() {
    if (DASH_OS_ANDROID) { return "android"; }
    if (DASH_OS_IOS    ) { return "ios"    ; }
    if (DASH_OS_WIN32  ) { return "window" ; }
    if (DASH_OS_OSX    ) { return "macos"  ; }
    if (DASH_OS_LINUX  ) { return "linux"  ; }
    
    return "";
}

define_reflectable_function(MInjectFunction, "args:clsName,fcnName,func")
define_reflectable_function(MSetClassSymbol, "args:obj,symName")
define_reflectable_function(MGetOS)

static void OnException(const std::string &message) {
    MPrint(message);
}

static void EnrollClassFunctions(
    MJsVM *vm, const std::string &clsName, const std::map<std::string, reflect::variable> &funcs)
{
    for (auto &pair : funcs) {
        const std::string  &name = pair.first;
        const reflect::any &func = pair.second.value;

        std::string fullName = clsName + "_" + name;
        vm->registerFunction(fullName, func);
    }
}

static void RegisterNativeFunctions(MJsVM *vm) {
    //class static and instance functions:
    for (auto &pair : *reflect::classes()) {
        const std::string &name = pair.first;
        auto meta = (reflect::class_meta *)reflect::query_type_meta(pair.second.type);

        EnrollClassFunctions(vm, name, meta->static_functions);
        EnrollClassFunctions(vm, name, meta->inst_functions  );
    }

    //global functions.
    for (auto &pair : *reflect::functions()) {
        const std::string  &name = pair.first;
        const reflect::any &func = pair.second.value;

        vm->registerFunction(name, func);
    }
}

static void RegisterBuiltinScript(MJsVM *vm) {
#define JS_(text) #text

    const char *builtin =
    JS_(
        class console {
            static log(message) {
                MPrint(message)
            }
        }
    );

#undef  JS_

    vm->evaluate("builtin", builtin);
}

static void launch() m_app_launch(launch) {
    //exception process.
    MJsVM *vm = MJsVM::instance();
    vm->setExceptionListener(MF(OnException));
    
    //set runtime environment.
    RegisterNativeFunctions(vm);
    RegisterBuiltinScript(vm);

    //launch.
    std::string name = "bundle.js";
    std::string code = MFileManager::instance()->u8stringFromBundle(name);
    vm->evaluate(name, code);
}
