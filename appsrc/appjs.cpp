#include "minikit.h"

//generic functions:

static MBaseVector::ptr MVector_create(const std::string &vType) {
    if (vType == "number") { return MVector<double     >::create(); }
    if (vType == "string") { return MVector<std::string>::create(); }
    
    return nullptr;
}

static MBaseMap::ptr MMap_string_create(const std::string &vType) {
    if (vType == "number") { return MMap<std::string, double     >::create(); }
    if (vType == "string") { return MMap<std::string, std::string>::create(); }
    
    return nullptr;
}

static MBaseMap::ptr MMap_number_create(const std::string &vType) {
    if (vType == "number") { return MMap<double, double     >::create(); }
    if (vType == "string") { return MMap<double, std::string>::create(); }
    
    return nullptr;
}

static MBaseMap::ptr MMap_create(const std::string &kType, const std::string &vType) {
    if (kType == "string") { return MMap_string_create(vType); }
    if (kType == "number") { return MMap_number_create(vType); }
    
    return nullptr;
}

static MBaseSet::ptr MSet_create(const std::string &vType) {
    if (vType == "number") { return MSet<double     >::create(); }
    if (vType == "string") { return MSet<std::string>::create(); }
    
    return nullptr;
}

define_reflectable_function(MVector_create, "ignore")
define_reflectable_function(MMap_create   , "ignore")
define_reflectable_function(MSet_create   , "ignore")

//runtime functions:

static void MSetObjectClassSymbol(const reflect::injectable::ptr &obj, const char *clsName) {
    if (obj) {
        reflect::symbol sym = reflect::symbol::make(clsName);
        obj->set_class_symbol(sym);
    }
}

static void MInjectClassFunction(
    const char *clsName, const char *fcnName, const reflect::base_function::ptr &func)
{
    reflect::inject(clsName, fcnName, func);
}

static std::string MMetaJsonDescription() {
    return reflect::meta_json_description();
}

static std::string MGetOS() {
    if (D_OS_ANDROID) { return "android"; }
    if (D_OS_IOS    ) { return "ios"    ; }
    if (D_OS_WIN32  ) { return "window" ; }
    if (D_OS_OSX    ) { return "macos"  ; }
    if (D_OS_LINUX  ) { return "linux"  ; }
    
    return "";
}

define_reflectable_function(MSetObjectClassSymbol, "ignore")
define_reflectable_function(MInjectClassFunction , "ignore")
define_reflectable_function(MMetaJsonDescription , "ignore")
define_reflectable_function(MGetOS               , "ignore")

//virtual machine:

static void OnException(const std::string &message) {
    MPrint(message);
}

static void EnrollClassFunctions(
    MJsVM *vm, const std::string &clsName, const std::map<std::string, reflect::variable> &funcs)
{
    for (auto &pair : funcs) {
        const std::string  &fcnName = pair.first;
        const reflect::any &fcnImpl = pair.second.value;

        std::string fullName = clsName + "_" + fcnName;
        vm->registerFunction(fullName, fcnImpl);
    }
}

static void RegisterNativeFunctions(MJsVM *vm) {
    //class static and instance functions:
    for (auto &pair : *reflect::classes()) {
        const std::string &clsName = pair.first;
        auto clsMeta = (reflect::class_meta *)reflect::query_type_meta(pair.second.type);

        EnrollClassFunctions(vm, clsName, clsMeta->static_functions);
        EnrollClassFunctions(vm, clsName, clsMeta->inst_functions  );
    }

    //global functions.
    for (auto &pair : *reflect::functions()) {
        const std::string  &fcnName = pair.first;
        const reflect::any &fcnImpl = pair.second.value;

        vm->registerFunction(fcnName, fcnImpl);
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
