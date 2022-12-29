#include "minikit.h"

//runtime functions:

static void MSetObjectClassSymbol(const reflect::injectable::ptr &obj, const char *clsSym) {
    if (obj) {
        reflect::symbol sym = reflect::symbol::make(clsSym);
        obj->set_class_symbol(sym);
    }
}

static std::string MGetObjectClassSymbol(const reflect::object::ptr &obj) {
    if (obj) {
        return obj->class_symbol().str();
    }
    return "";
}

static void MInjectClassFunction(
    const char *clsName, const char *fcnName, const reflect::generic_function::ptr &func)
{
    reflect::inject(clsName, fcnName, func);
}

static std::string MMetaJsonDescription() {
    return reflect::meta_json_description();
}

static void MWriteTextFile(const std::string &path, const std::string &content) {
    MFileManager::instance()->writeU8StringToFile(content, path);
}

define_reflectable_function(MSetObjectClassSymbol, "ignore")
define_reflectable_function(MGetObjectClassSymbol, "ignore")
define_reflectable_function(MInjectClassFunction , "ignore")
define_reflectable_function(MMetaJsonDescription , "ignore")
define_reflectable_function(MWriteTextFile       , "ignore")

//virtual machine:

static void OnException(const std::string &message) {
    MPrint(message);
}

static void EnrollClassFunctions(
    MJsVM *vm, const std::string &clsName, const std::map<std::string, reflect::variable> &funcs)
{
    for (auto &[fcnName, fcnImpl] : funcs) {
        std::string fullName = clsName + "_" + fcnName;
        vm->registerFunction(fullName, fcnImpl.value);
    }
}

static void RegisterNativeFunctions(MJsVM *vm) {
    //class static and instance functions:
    for (auto &[clsName, clsDesc] : *reflect::classes()) {
        auto clsMeta = (reflect::class_meta *)reflect::query_type_meta(clsDesc.type);

        EnrollClassFunctions(vm, clsName, clsMeta->static_functions);
        EnrollClassFunctions(vm, clsName, clsMeta->inst_functions  );
    }

    //global functions.
    for (auto &[fcnName, fcnImpl] : *reflect::functions()) {
        vm->registerFunction(fcnName, fcnImpl.value);
    }
}

static void RegisterBuiltinScript(MJsVM *vm) {
#define JS_(text) #text

    const char *builtin =
    JS_(
        class console {
            static log(message) {
                MPrint(String(message))
            }
        }
    );

#undef  JS_

    vm->evaluate("builtin", builtin);
}

static void launch() m_app_launch(launch, 10) {
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
