#pragma once

#include "rmeta.h"

//NOTE: an explicit template specialization must be declared
//in the namespace that includes the template (although msvc and clang have not this limitation).
//so 'declare_reflectable_xx' only can be used in the global scope.

#define declare_reflectable_class(Name)                             \
/**/    class Name;                                                 \
/**/    template<> struct reflect::type_ids<Name> {                 \
/**/        static constexpr const void *ids[] = { #Name, nullptr };\
/**/    };

#define declare_reflectable_enum(Name)                              \
/**/    enum class Name;                                            \
/**/    template<> struct reflect::type_ids<Name> {                 \
/**/        static constexpr const void *ids[] = { #Name, nullptr };\
/**/    };

#define define_reflectable_const(Const)                             \
/**/    static reflect::committor _unused_##Const(#Const, Const);   \

#define define_reflectable_function(Function, ...)                  \
/**/    static reflect::committor _unused_##Function(               \
/**/        #Function, Function, ##__VA_ARGS__                      \
/**/    );

#define define_reflectable_class_const(Class, Const)                \
/**/    static reflect::committor _unused_##Class##_##Const(        \
/**/        reflect::type_symbol<Class>::value(),                   \
/**/        #Const,                                                 \
/**/        Class::Const                                            \
/**/    );

#define define_reflectable_class_function(Class, Function, ...)     \
/**/    static reflect::committor _unused_##Class##_##Function(     \
/**/        reflect::type_symbol<Class>::value(),                   \
/**/        #Function,                                              \
/**/        &Class::Function,                                       \
/**/        ##__VA_ARGS__                                           \
/**/    );

#define define_reflectable_enum_const(Enum, Const)                  \
/**/    static reflect::committor _unused_##Enum##_##Const(         \
/**/        0,                                                      \
/**/        reflect::type_symbol<Enum>::value(),                    \
/**/        #Const,                                                 \
/**/        (int)Enum::Const                                        \
/**/    );

#define implement_injectable_function(Ret, ...)                     \
/**/    if (auto f = find_injected(__func__)) {                     \
/**/        return Ret f->call_with_args({ this, ##__VA_ARGS__ });  \
/**/    }

namespace reflect {

struct committor {
    //global constant:
    committor(const char *name, const char *value) noexcept {
        auto type = type_symbol<std::string>::value();
        commit_type_meta(type, type_category::is_string);
        commit_variable(name, type, std::string(value));
    }

    committor(const char *name, double value) noexcept {
        auto type = type_symbol<double>::value();
        commit_type_meta(type, type_category::is_double);
        commit_variable(name, type, value);
    }

    committor(const char *name, float value) noexcept {
        auto type = type_symbol<float>::value();
        commit_type_meta(type, type_category::is_float);
        commit_variable(name, type, value);
    }

    committor(const char *name, int64_t value) noexcept {
        auto type = type_symbol<int64_t>::value();
        commit_type_meta(type, type_category::is_int64);
        commit_variable(name, type, value);
    }

    committor(const char *name, int value) noexcept {
        auto type = type_symbol<int>::value();
        commit_type_meta(type, type_category::is_int);
        commit_variable(name, type, value);
    }

    //global function.
    template<class Ret, class... Args> committor(
        const char *name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
        auto type = symbol::make(name);
        commit_type_meta(type, type_category::is_function);
        commit_function(name, type, 0);
    }

    //class static constant:
    committor(const symbol &cls, const char *name, const char *value) noexcept {
        commit_type_meta(cls, type_category::is_class);
        commit_class(cls.str(), cls);

        auto type = type_symbol<std::string>::value();
        commit_type_meta(type, type_category::is_string);
        commit_variable(name, type, std::string(value));
    }

    committor(const symbol &cls, const char *name, double value) noexcept {
        commit_type_meta(cls, type_category::is_class);
        commit_class(cls.str(), cls);

        auto type = type_symbol<double>::value();
        commit_type_meta(type, type_category::is_double);
        commit_variable(name, type, value);
    }

    committor(const symbol &cls, const char *name, float value) noexcept {
        commit_type_meta(cls, type_category::is_class);
        commit_class(cls.str(), cls);

        auto type = type_symbol<float>::value();
        commit_type_meta(type, type_category::is_float);
        commit_variable(name, type, value);
    }

    committor(const symbol &cls, const char *name, int64_t value) noexcept {
        commit_type_meta(cls, type_category::is_class);
        commit_class(cls.str(), cls);

        auto type = type_symbol<int64_t>::value();
        commit_type_meta(type, type_category::is_int64);
        commit_variable(name, type, value);
    }

    committor(const symbol &cls, const char *name, int value) noexcept {
        commit_type_meta(cls, type_category::is_class);
        commit_class(cls.str(), cls);

        auto type = type_symbol<int>::value();
        commit_type_meta(type, type_category::is_int);
        commit_variable(name, type, value);
    }

    //class static function.
    template<class Ret, class... Args> committor(
        const symbol &cls, const char *name, Ret (*fcn)(Args...),
        const char *note = nullptr) noexcept
    {
        commit_type_meta(cls, type_category::is_class);
        commit_class(cls.str(), cls);
    }

    //class instance function.
    template<class Ret, class Class, class... Args> committor(
        const symbol &cls, const char *name, Ret (Class::*fcn)(Args...),
        const char *note = nullptr) noexcept
    {
        commit_type_meta(cls, type_category::is_class);
        commit_class(cls.str(), cls);
    }

    //enumeration value.
    committor(int, const symbol &enu, const char *name, int value) noexcept {
        commit_type_meta(enu, type_category::is_enum);
        commit_enum(enu.str(), enu);

        auto type = type_symbol<int>::value();
        commit_type_meta(type, type_category::is_int);
        commit_enum_value(enu, name, type, value);
    }
};

} //end reflect.
