#pragma once

#include "rset.h"

//generation mark:
//

#define reflectable_class(Name)                                 \
/**/    template<> struct reflect::type_name_of<class Name> {   \
/**/        static constexpr const char *const name = #Name;    \
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

//categoroies:
//

enum class type_category {
    is_void  ,  //void.
    is_bool  ,  //bool.
    is_int   ,  //int.
    is_int64 ,  //int64_t.
    is_float ,  //float.
    is_double,  //double.
    string   ,  //std::string.
    object   ,  //reflect::object.
    function ,  //reflect::function<ret (args...)>.
    map      ,  //reflect::map<key, value>.
    set      ,  //reflect::set<value>.
    vector   ,  //reflect::vector<value>.
};

enum class form_category {
    value       ,   //value.
    const_ptr   ,   //const value *
    ptr         ,   //value *.
    const_ref   ,   //const value &.
    ref         ,   //value &.
    const_shared,   //const std::shared_ptr<value> &.
    shared      ,   //std::shared_ptr<value>.
};

//builtin types:
//

template<> struct type_name_of<void       > { static constexpr const char *const name = "void"  ; };
template<> struct type_name_of<bool       > { static constexpr const char *const name = "bool"  ; };
template<> struct type_name_of<int        > { static constexpr const char *const name = "int"   ; };
template<> struct type_name_of<int64_t    > { static constexpr const char *const name = "int64" ; };
template<> struct type_name_of<float      > { static constexpr const char *const name = "float" ; };
template<> struct type_name_of<double     > { static constexpr const char *const name = "double"; };
template<> struct type_name_of<std::string> { static constexpr const char *const name = "string"; };

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
