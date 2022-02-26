#pragma once

#include "rmap.h"
#include "rset.h"
#include "rvector.h"

//generation mark:
//

#define reflectable_class(...)                                          \
/**/    template<> struct reflect::type_name_of<class __VA_ARGS__> {    \
/**/        static constexpr const char *const name = #__VA_ARGS__;     \
/**/    };

#define reflectable_class_function(Class, Function)                         \
/**/    static reflect::function_commit _unused_var_##Class##_##Function(   \
/**/        #Class, #Function, &Class::Function                             \
/**/    );

#define reflectable_function(Function)                          \
/**/    static reflect::function_commit _unused_var_##Function( \
/**/        #Function, Function                                 \
/**/    );

namespace reflect {

//builtin types:
//

template<> struct type_name_of<void       > { static constexpr const char *const name = "void"  ; };
template<> struct type_name_of<bool       > { static constexpr const char *const name = "bool"  ; };
template<> struct type_name_of<int        > { static constexpr const char *const name = "int"   ; };
template<> struct type_name_of<int64_t    > { static constexpr const char *const name = "int64" ; };
template<> struct type_name_of<float      > { static constexpr const char *const name = "float" ; };
template<> struct type_name_of<double     > { static constexpr const char *const name = "double"; };
template<> struct type_name_of<std::string> { static constexpr const char *const name = "string"; };

reflectable_class(vector<bool>       )
reflectable_class(vector<int>        )
reflectable_class(vector<int64_t>    )
reflectable_class(vector<float>      )
reflectable_class(vector<double>     )
reflectable_class(vector<std::string>)

reflectable_class(map<std::string, bool>       )
reflectable_class(map<std::string, int>        )
reflectable_class(map<std::string, int64_t>    )
reflectable_class(map<std::string, float>      )
reflectable_class(map<std::string, double>     )
reflectable_class(map<std::string, std::string>)

reflectable_class(map<int, bool>       )
reflectable_class(map<int, int>        )
reflectable_class(map<int, int64_t>    )
reflectable_class(map<int, float>      )
reflectable_class(map<int, double>     )
reflectable_class(map<int, std::string>)

reflectable_class(set<bool>       )
reflectable_class(set<int>        )
reflectable_class(set<int64_t>    )
reflectable_class(set<float>      )
reflectable_class(set<double>     )
reflectable_class(set<std::string>)

//function extraction:
//

struct function_commit {
    template<class Ret, class Class, class... Args> function_commit(
        const char *const class_name, const char *fcn_name, Ret (Class::*fcn)(Args...))
    {
    }

    template<class Ret, class... Args> function_commit(
        const char *const class_name, const char *fcn_name, Ret (*fcn)(Args...))
    {
    }

    template<class Ret, class... Args> function_commit(
        const char *fcn_name, Ret (*fcn)(Args...))
    {
    }
};

} // end reflect.
