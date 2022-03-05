#pragma once

#include "rmap.h"
#include "rset.h"
#include "rvector.h"

//generation mark:
//

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

#define define_reflectable_enum_const(Enum, Const)                  \
/**/    static reflect::committor _unused_##Enum##_##Const(         \
/**/        #Enum, #Const, (int)Enum::Const                         \
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
//

template<> struct type_name_of<void       > { static constexpr const char *const name = "void"  ; };
template<> struct type_name_of<bool       > { static constexpr const char *const name = "bool"  ; };
template<> struct type_name_of<uint8_t    > { static constexpr const char *const name = "byte"  ; };
template<> struct type_name_of<int        > { static constexpr const char *const name = "int"   ; };
template<> struct type_name_of<int64_t    > { static constexpr const char *const name = "int64" ; };
template<> struct type_name_of<float      > { static constexpr const char *const name = "float" ; };
template<> struct type_name_of<double     > { static constexpr const char *const name = "double"; };
template<> struct type_name_of<std::string> { static constexpr const char *const name = "string"; };

declare_reflectable_class(vector<bool>       )
declare_reflectable_class(vector<uint8_t>    )
declare_reflectable_class(vector<int>        )
declare_reflectable_class(vector<int64_t>    )
declare_reflectable_class(vector<float>      )
declare_reflectable_class(vector<double>     )
declare_reflectable_class(vector<std::string>)

declare_reflectable_class(map<std::string, bool>       )
declare_reflectable_class(map<std::string, int>        )
declare_reflectable_class(map<std::string, int64_t>    )
declare_reflectable_class(map<std::string, float>      )
declare_reflectable_class(map<std::string, double>     )
declare_reflectable_class(map<std::string, std::string>)
declare_reflectable_class(map<int        , bool>       )
declare_reflectable_class(map<int        , int>        )
declare_reflectable_class(map<int        , int64_t>    )
declare_reflectable_class(map<int        , float>      )
declare_reflectable_class(map<int        , double>     )
declare_reflectable_class(map<int        , std::string>)

declare_reflectable_class(set<bool>       )
declare_reflectable_class(set<int>        )
declare_reflectable_class(set<int64_t>    )
declare_reflectable_class(set<float>      )
declare_reflectable_class(set<double>     )
declare_reflectable_class(set<std::string>)

//type extraction:
//

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

    //enumeration constant.
    committor(const char *enum_name, const char *value_name, int value) {
    }

    //string constant.
    committor(const char *name, const char *value) {
    }

    //double constant.
    committor(const char *name, double value) {
    }

    //float constant.
    committor(const char *name, float value) {
    }

    //int64 constant.
    committor(const char *name, int64_t value) {
    }

    //int constant.
    committor(const char *name, int value) {
    }
};

} // end reflect.
