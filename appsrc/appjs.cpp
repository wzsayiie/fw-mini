#include "minikit.h"

static void onException(const std::string &message) {
    MPrint(message);
}

static void registerClassFunctions(
    MJsVM *vm, const std::string &cls_name, const std::map<std::string, reflect::variable> &funcs)
{
    for (auto &pair : funcs) {
        const std::string  &name = pair.first;
        const reflect::any &func = pair.second.value;

        std::string full_name = cls_name + "_" + name;
        vm->registerFunction(full_name, func);
    }
}

static void registerNativeFunctions(MJsVM *vm) {
    //class static and instance functions:
    for (auto &pair : *reflect::classes()) {
        const std::string &name = pair.first;
        auto meta = (reflect::class_meta *)reflect::query_type_meta(pair.second.type);

        registerClassFunctions(vm, name, meta->static_functions);
        registerClassFunctions(vm, name, meta->inst_functions  );
    }

    //global functions.
    for (auto &pair : *reflect::functions()) {
        const std::string  &name = pair.first;
        const reflect::any &func = pair.second.value;

        vm->registerFunction(name, func);
    }
}

static void registerNativeMetas(MJsVM *vm) {
}

static void registerBuiltinFunctions(MJsVM *vm) {
}

static void registerBuiltinScript(MJsVM *vm) {
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
    //exception handle.
    MJsVM *vm = MJsVM::instance();
    vm->setExceptionListener(MF(onException));
    
    //set runtime environment.
    registerNativeFunctions(vm);
    registerNativeMetas(vm);
    registerBuiltinFunctions(vm);
    registerBuiltinScript(vm);

    //launch.
    std::string name = "bundle.js";
    std::string code = MFileManager::instance()->u8stringFromBundle(name);
    vm->evaluate(name, code);
}
