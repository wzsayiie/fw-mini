#pragma once

#include "rextraction.h"

//NOTE: an explicit template specialization must be declared
//in the namespace that includes the template (although msvc and clang have not this limitation).
//so 'declare_reflectable_xx' only can be used in the global scope.

#define declare_reflectable_class(Name)                                     \
/**/    class Name;                                                         \
/**/    template<> struct reflect::typeids_of<Name> {                       \
/**/        static constexpr const void *value[] = { #Name, nullptr };      \
/**/    };

#define declare_reflectable_enum(Name)                                      \
/**/    enum class Name;                                                    \
/**/    template<> struct reflect::typeids_of<Name> {                       \
/**/        static constexpr const void *value[] = { #Name, nullptr };      \
/**/    };

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
/**/    if (auto f = find_injected(__func__)) {                             \
/**/        auto r = f->call_with_args({ this, ##__VA_ARGS__ });            \
/**/        return reflect::take<Ret>::from(r);                             \
/**/    }

namespace reflect {

class generator {
public:
    //global constant:
    generator(const char *n, std::string &v) noexcept { commit_variable(n, symbol_of<std::string>::value(), v); }
    generator(const char *n, double       v) noexcept { commit_variable(n, symbol_of<double     >::value(), v); }
    generator(const char *n, float        v) noexcept { commit_variable(n, symbol_of<float      >::value(), v); }
    generator(const char *n, int64_t      v) noexcept { commit_variable(n, symbol_of<int64_t    >::value(), v); }
    generator(const char *n, int          v) noexcept { commit_variable(n, symbol_of<int        >::value(), v); }

    //global function.
    template<class Ret, class... Args> generator(
        const char *name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
        symbol type = extract<typename function<Ret (Args...)>::ptr>::commit(note);
        
        commit_function(name, type, function<Ret (Args...)>::create([=](Args... args) {
            return fcn(args...);
        }));
    }

    //class static constant:
    template<class C> generator(C *, const char *n, const std::string &v) noexcept { cls_var<C, std::string>(n, v); }
    template<class C> generator(C *, const char *n, double             v) noexcept { cls_var<C, double     >(n, v); }
    template<class C> generator(C *, const char *n, float              v) noexcept { cls_var<C, float      >(n, v); }
    template<class C> generator(C *, const char *n, int64_t            v) noexcept { cls_var<C, int64_t    >(n, v); }
    template<class C> generator(C *, const char *n, int                v) noexcept { cls_var<C, int        >(n, v); }

    //class static function.
    template<class Ret, class Class, class... Args> generator(
        Class *, const char *name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
        symbol cls_type = extract<Class>::commit();
        symbol fcn_type = extract<typename function<Ret (Args...)>::ptr>::commit(note);
        
        commit_class_function(cls_type, name, fcn_type, function<Ret (Args...)>::create(
            [=](Args... args) {
                return fcn(args...);
            }
        ));
    }

    //class instance function.
    template<class Ret, class Class, class... Args> generator(
        Class *, const char *name, Ret (Class::*fcn)(Args...), const char *note = nullptr) noexcept
    {
        symbol cls_type = extract<Class>::commit();
        symbol fcn_type = extract<typename function<Ret (const typename Class::ptr &, Args...)>::ptr>::commit(note);
        
        commit_object_function(cls_type, name, fcn_type, function<Ret (const typename Class::ptr &, Args...)>::create(
            [=](const typename Class::ptr &self, Args... args) {
                return (self.get()->*fcn)(args...);
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
        symbol cls_type = extract<Class>::commit();
        symbol val_type = symbol_of<Value>::value();

        commit_class(cls_type.str(), cls_type);
        commit_class_variable(cls_type, name, val_type, value);
    }
};

} //end reflect.
