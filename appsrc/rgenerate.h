#pragma once

#include "rmap.h"
#include "rset.h"
#include "rvector.h"

//generation mark:

//NOTE: an explicit template specialization must be declared
//in the namespace that includes the template (although msvc and clang have not this limitation).
//so 'declare_reflectable_xx' only can be used in the global scope.

#define declare_reflectable_class(...)                              \
/**/    template<> struct reflect::type_name_of<class __VA_ARGS__> {\
/**/        static constexpr const char *const name = #__VA_ARGS__; \
/**/    };

#define declare_reflectable_enum(Name)                              \
/**/    enum class Name;                                            \
/**/    template<> struct reflect::type_name_of<Name> {             \
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
/**/    static reflect::committor _unused_##Name(#Const, Const);    \

#define implement_injectable_function(Ret, ...)                     \
/**/    if (auto f = find_injected(__func__)) {                     \
/**/        auto self = std::static_pointer_cast<dash::object>(     \
/**/            reflect::shared(this)                               \
/**/        );                                                      \
/**/        return Ret f->call_with_args({ self, ##__VA_ARGS__ });  \
/**/    }

namespace reflect {

//builtin types:

template<> struct type_name_of<void       > { static constexpr const char *const name = "void"  ; };
template<> struct type_name_of<bool       > { static constexpr const char *const name = "bool"  ; };
template<> struct type_name_of<uint8_t    > { static constexpr const char *const name = "byte"  ; };
template<> struct type_name_of<int        > { static constexpr const char *const name = "int"   ; };
template<> struct type_name_of<int64_t    > { static constexpr const char *const name = "int64" ; };
template<> struct type_name_of<float      > { static constexpr const char *const name = "float" ; };
template<> struct type_name_of<double     > { static constexpr const char *const name = "double"; };
template<> struct type_name_of<std::string> { static constexpr const char *const name = "string"; };

//type extraction:

struct committor {
    //class instance function.
    template<class Ret, class Class, class... Args> committor(
        const char *class_name, const char *fcn_name, Ret (Class::*fcn)(Args...), const char *note = nullptr)
    {
    }

    //class static function.
    template<class Ret, class... Args> committor(
        const char *class_name, const char *fcn_name, Ret (*fcn)(Args...), const char *note = nullptr)
    {
    }

    //global function.
    template<class Ret, class... Args> committor(
        const char *fcn_name, Ret (*fcn)(Args...), const char *note = nullptr)
    {
    }

    //class constant member.
    committor(const char *class_name, const char *value_name, const char *value) {
    }
    committor(const char *class_name, const char *value_name, double value) {
    }
    committor(const char *class_name, const char *value_name, float value) {
    }
    committor(const char *class_name, const char *value_name, int64_t value) {
    }
    committor(const char *class_name, const char *value_name, int value) {
    }

    //enumeration member.
    committor(int, const char *enum_name, const char *value_name, int64_t value) {
    }

    //global constant.
    committor(const char *name, const char *value) {
    }
    committor(const char *name, double value) {
    }
    committor(const char *name, float value) {
    }
    committor(const char *name, int64_t value) {
    }
    committor(const char *name, int value) {
    }
};

} //end reflect.

//builtin types:

declare_reflectable_class(reflect::vector<bool>       )
declare_reflectable_class(reflect::vector<uint8_t>    )
declare_reflectable_class(reflect::vector<int>        )
declare_reflectable_class(reflect::vector<int64_t>    )
declare_reflectable_class(reflect::vector<float>      )
declare_reflectable_class(reflect::vector<double>     )
declare_reflectable_class(reflect::vector<std::string>)

declare_reflectable_class(reflect::map<std::string, bool>       )
declare_reflectable_class(reflect::map<std::string, int>        )
declare_reflectable_class(reflect::map<std::string, int64_t>    )
declare_reflectable_class(reflect::map<std::string, float>      )
declare_reflectable_class(reflect::map<std::string, double>     )
declare_reflectable_class(reflect::map<std::string, std::string>)
declare_reflectable_class(reflect::map<int        , bool>       )
declare_reflectable_class(reflect::map<int        , int>        )
declare_reflectable_class(reflect::map<int        , int64_t>    )
declare_reflectable_class(reflect::map<int        , float>      )
declare_reflectable_class(reflect::map<int        , double>     )
declare_reflectable_class(reflect::map<int        , std::string>)

declare_reflectable_class(reflect::set<bool>       )
declare_reflectable_class(reflect::set<int>        )
declare_reflectable_class(reflect::set<int64_t>    )
declare_reflectable_class(reflect::set<float>      )
declare_reflectable_class(reflect::set<double>     )
declare_reflectable_class(reflect::set<std::string>)

declare_reflectable_class(reflect::function<void ()>)
