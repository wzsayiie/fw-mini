#include "minikit.h"

//runtime functions:

static std::string MGetObjectClassSymbol(const reflect::object::ptr &obj) {
    return obj->class_symbol().str();
}

static void MInjectObjectFunction(
    const reflect::injectable::ptr       &obj ,
    const char                           *name,
    const reflect::generic_function::ptr &func)
{
    obj->inject_function(name, func);
}

static void MDisposeObject(const reflect::injectable::ptr &obj) {
    obj->dispose();
}

define_reflectable_function(MGetObjectClassSymbol, "ignore")
define_reflectable_function(MInjectObjectFunction, "ignore")
define_reflectable_function(MDisposeObject       , "ignore")

//development environment functions:

static std::string metaJsonDescription() {
    return reflect::meta_json_description();
}

static std::string temporaryDirectory() {
    return MBundle::instance()->temporaryDirectory();
}

static void writeTextFile(const std::string &path, const std::string &content) {
    MFileManager::instance()->writeU8StringToFile(content, path);
}

define_reflectable_function(metaJsonDescription, "ignore")
define_reflectable_function(temporaryDirectory , "ignore")
define_reflectable_function(writeTextFile      , "ignore")

//virtual machine:

static void OnException(const std::string &message) {
    MPrint(message);
}

static void EnrollClassFunctions(
    MJsVM *vm, const std::string &clsName, const std::map<std::string, reflect::variable> &funcs)
{
    for (auto &[fcnName, fcnImpl] : funcs) {
        std::string fullName = "native_" + clsName + "_" + fcnName;
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
        std::string fullName = "native_" + fcnName;
        vm->registerFunction(fullName, fcnImpl.value);
    }
}

static void RegisterBuiltinScript(MJsVM *vm) {
#define JS_(text) #text

    const char *builtin =
    JS_(
        class console {
            static log(message) {
                native_MPrint(String(message))
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
