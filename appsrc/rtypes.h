#pragma once

#include "rset.h"

namespace reflect {

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

} // end reflect.
