#pragma once

#include "rmeta.h"

//NOTE: an explicit template specialization must be declared
//in the namespace that includes the template (although msvc and clang have not this limitation).
//so 'declare_reflectable_xx' only can be used in the global scope.

#define declare_reflectable_special(...)                            \
/**/    template<> struct reflect::type_name<__VA_ARGS__> {         \
/**/        static constexpr const char *const name = #__VA_ARGS__; \
/**/    };

#define declare_reflectable_class(Name)                             \
/**/    class Name;                                                 \
/**/    template<> struct reflect::type_name<Name> {                \
/**/        static constexpr const char *const name = #Name;        \
/**/    };

#define declare_reflectable_enum(Name)                              \
/**/    enum class Name;                                            \
/**/    template<> struct reflect::type_name<Name> {                \
/**/        static constexpr const char *const name = #Name;        \
/**/    };

#define define_reflectable_class_function(Class, Function, ...)     \
/**/    static reflect::committor _unused_##Class##_##Function(     \
/**/        #Class, #Function, &Class::Function, ##__VA_ARGS__      \
/**/    );

#define define_reflectable_function(Function, ...)                  \
/**/    static reflect::committor _unused_##Function(               \
/**/        #Function, Function, ##__VA_ARGS__                      \
/**/    );

#define define_reflectable_class_const(Class, Const)                \
/**/    static reflect::committor _unused_##Class##_##Const(        \
/**/        #Class, #Const, Class::Const                            \
/**/    );

#define define_reflectable_enum_const(Enum, Const)                  \
/**/    static reflect::committor _unused_##Enum##_##Const(         \
/**/        0, #Enum, #Const, (int)Enum::Const                      \
/**/    );

#define define_reflectable_const(Const)                             \
/**/    static reflect::committor _unused_##Const(#Const, Const);   \

#define implement_injectable_function(Ret, ...)                     \
/**/    if (auto f = find_injected(__func__)) {                     \
/**/        auto self = shared();                                   \
/**/        return Ret f->call_with_args({ self, ##__VA_ARGS__ });  \
/**/    }

namespace reflect {

struct committor {
    //class instance function.
    template<class Ret, class Class, class... Args> committor(
        const char *class_name, const char *fcn_name, Ret (Class::*fcn)(Args...), const char *note = nullptr) noexcept
    {
    }

    //class static function.
    template<class Ret, class... Args> committor(
        const char *class_name, const char *fcn_name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
    }

    //global function.
    template<class Ret, class... Args> committor(
        const char *fcn_name, Ret (*fcn)(Args...), const char *note = nullptr) noexcept
    {
    }

    //class constant member.
    committor(const char *class_name, const char *value_name, const char *value) noexcept {
    }
    committor(const char *class_name, const char *value_name, double value) noexcept {
    }
    committor(const char *class_name, const char *value_name, float value) noexcept {
    }
    committor(const char *class_name, const char *value_name, int64_t value) noexcept {
    }
    committor(const char *class_name, const char *value_name, int value) noexcept {
    }

    //enumeration member.
    committor(int, const char *enum_name, const char *value_name, int64_t value) noexcept {
    }

    //global constant.
    committor(const char *name, const char *value) noexcept {
    }
    committor(const char *name, double value) noexcept {
    }
    committor(const char *name, float value) noexcept {
    }
    committor(const char *name, int64_t value) noexcept {
    }
    committor(const char *name, int value) noexcept {
    }
};

} //end reflect.
