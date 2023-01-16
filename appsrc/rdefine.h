#pragma once

#include "rextract.h"

#define define_reflectable_const(Const)                                     \
/**/    static reflect::generator _unused_##Const(#Const, Const);           \

#define define_reflectable_function(Function, ...)                          \
/**/    static reflect::generator _unused_##Function(                       \
/**/        #Function, Function, ##__VA_ARGS__                              \
/**/    );

#define define_reflectable_class_const(Class, Const)                        \
/**/    static reflect::generator _unused_##Class##_##Const(                \
/**/        (Class *)nullptr, #Const, Class::Const                          \
/**/    );

#define define_reflectable_class_function(Class, Function, ...)             \
/**/    static reflect::generator _unused_##Class##_##Function(             \
/**/        (Class *)nullptr, #Function, &Class::Function, ##__VA_ARGS__    \
/**/    );

#define define_reflectable_enum_const(Enum, Const)                          \
/**/    static reflect::generator _unused_##Enum##_##Const(                 \
/**/        (Enum)0, #Const, (int)Enum::Const                               \
/**/    );

#define implement_injectable_function(Ret, ...)                             \
/**/    {                                                                   \
/**/        static bool entrance = true;                                    \
/**/        if (entrance) {                                                 \
/**/        if (auto fcn = find_injected_function(__func__)) {              \
/**/            entrance = false;                                           \
/**/            auto ret = fcn->call_with_args({ this, ##__VA_ARGS__ });    \
/**/            entrance = true;                                            \
/**/            return reflect::take<Ret>::from(ret);                       \
/**/        }}                                                              \
/**/    }

namespace reflect {

class generator {
public:
    //global constant:
    generator(const char *n, const char *v) noexcept { commit_variable(n, symbol_of<std::string>::value(), v); }
    generator(const char *n, double      v) noexcept { commit_variable(n, symbol_of<double     >::value(), v); }
    generator(const char *n, float       v) noexcept { commit_variable(n, symbol_of<float      >::value(), v); }
    generator(const char *n, int64_t     v) noexcept { commit_variable(n, symbol_of<int64_t    >::value(), v); }
    generator(const char *n, int         v) noexcept { commit_variable(n, symbol_of<int        >::value(), v); }

    //global function.
    template<class Ret, class... Args> generator(
        const char *name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
        symbol type = extract<typename function<Ret (Args...)>::ptr>::commit();
        
        commit_function(name, type, note, function<Ret (Args...)>::create([=](Args... args) {
            return fcn(args...);
        }));
    }

    //class static constant:
    template<class C> generator(C *, const char *n, const char *v) noexcept { cls_var<C, std::string>(n, v); }
    template<class C> generator(C *, const char *n, double      v) noexcept { cls_var<C, double     >(n, v); }
    template<class C> generator(C *, const char *n, float       v) noexcept { cls_var<C, float      >(n, v); }
    template<class C> generator(C *, const char *n, int64_t     v) noexcept { cls_var<C, int64_t    >(n, v); }
    template<class C> generator(C *, const char *n, int         v) noexcept { cls_var<C, int        >(n, v); }

    //class static function.
    template<class Ret, class Class, class... Args> generator(
        Class *, const char *name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
        symbol bas_type = extract<typename base_of<Class>::type>::commit();
        symbol cls_type = extract<typename Class::ptr>::commit();
        symbol fcn_type = extract<typename function<Ret (Args...)>::ptr>::commit();
        
        commit_class(bas_type.str(), bas_type);
        commit_class(cls_type.str(), cls_type);
        commit_class_static_function(cls_type, name, fcn_type, note, function<Ret (Args...)>::create(
            [=](Args... args) {
                return fcn(args...);
            }
        ));
    }

    //class instance function.
    template<class Ret, class Class, class... Args> generator(
        Class *, const char *name, Ret (Class::*fcn)(Args...), const char *note = nullptr) noexcept
    {
        symbol bas_type = extract<typename base_of<Class>::type>::commit();
        symbol cls_type = extract<typename Class::ptr>::commit();
        symbol fcn_type = extract<typename function<Ret (const typename Class::ptr &, Args...)>::ptr>::commit();
        
        commit_class(bas_type.str(), bas_type);
        commit_class(cls_type.str(), cls_type);
        commit_class_inst_function(cls_type, name, fcn_type, note, function<Ret (const typename Class::ptr &, Args...)>::create(
            [=](const typename Class::ptr &thisArg, Args... args) {
                return (thisArg.get()->*fcn)(args...);
            }
        ));
    }

    //enumeration value.
    template<class Enum> generator(Enum, const char *name, int value) noexcept {
        symbol enum_type = extract<Enum>::commit();
        symbol memb_type = symbol_of<int>::value();

        commit_enum(enum_type.str(), enum_type);
        commit_enum_value(enum_type, name, memb_type, value);
    }
    
private:
    template<class Class, class Value> void cls_var(const char *name, const any &value) {
        symbol bas_type = extract<typename base_of<Class>::type>::commit();
        symbol cls_type = extract<typename Class::ptr>::commit();
        symbol val_type = symbol_of<Value>::value();

        commit_class(bas_type.str(), bas_type);
        commit_class(cls_type.str(), cls_type);
        commit_class_static_variable(cls_type, name, val_type, value);
    }
};

} //end reflect.
