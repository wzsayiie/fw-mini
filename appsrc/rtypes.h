#pragma once

#include "rutility.h"

namespace reflect {

enum class type_category {
    is_void  ,  //void.
    is_bool  ,  //bool.
    is_int32 ,  //int.
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

template<class> struct reflectable_type;

template<class Type> struct type_name {
    static symbol *value() {
        static symbol *sym = symbol::make(reflectable_type<Type>::name);
        return sym;
    }
};

template<> struct reflectable_type<void       > { static constexpr const char *const name = "void"  ; };
template<> struct reflectable_type<bool       > { static constexpr const char *const name = "bool"  ; };
template<> struct reflectable_type<int        > { static constexpr const char *const name = "int"   ; };
template<> struct reflectable_type<int64_t    > { static constexpr const char *const name = "int64" ; };
template<> struct reflectable_type<float      > { static constexpr const char *const name = "float" ; };
template<> struct reflectable_type<double     > { static constexpr const char *const name = "double"; };
template<> struct reflectable_type<std::string> { static constexpr const char *const name = "string"; };

} // end reflect.
