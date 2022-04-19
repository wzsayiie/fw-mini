#pragma once

#include "rfunction.h"

namespace reflect {

//type:
//

enum class type_category {
    is_void    ,    //void.
    is_bool    ,    //bool.
    is_byte    ,    //uint8_t.
    is_int     ,    //int.
    is_int64   ,    //int64_t.
    is_float   ,    //float.
    is_double  ,    //double.
    is_string  ,    //std::string.
    
    is_vector  ,    //reflect::vector<value>.
    is_map     ,    //reflect::map<key, value>.
    is_set     ,    //reflect::set<value>.

    is_function,    //reflect::function<ret (args...)>.
    is_class   ,    //reflect::object (exclude function, map, set and vector).

    is_const   ,    //"int" or "const char *" constant.
    is_enum    ,    //"int" enumeration.
};

enum class type_qualifier {
    value       ,   //value.
    const_ptr   ,   //const value *.
    ptr         ,   //value *.
    const_ref   ,   //const value &.
    ref         ,   //value &.
    const_shared,   //const std::shared_ptr<value> &.
    shared      ,   //std::shared_ptr<value>.
};

//meta:
//
//  +-- type
//  |   +-- const
//  |   +-- enum
//  |   +-- super
//  |   |   +-- vector
//  |   |   +-- map
//  |   |   +-- set
//  |   |   +-- function
//  |   |   +-- class
//

struct type_meta {
    type_category type = type_category::is_void;
    symbol       *name = nullptr;
};

struct const_meta : type_meta {
    symbol       *belong_class = nullptr;
    symbol       *belong_enum  = nullptr;
    type_category value_type   = type_category::is_void;
    const char   *string_value = nullptr;
    double        double_value = 0;
    int64_t       int64_value  = 0;
};

struct enum_meta : type_meta {
    std::map<symbol *, const_meta *> value_map;
    std::vector<const_meta *>        value_seq;
};

struct super_meta : type_meta {
    object::ptr (*create)() = nullptr;
};

struct vector_map : super_meta {
    symbol *val_type = nullptr;
};

struct map_meta : super_meta {
    symbol *key_type = nullptr;
    symbol *val_type = nullptr;
};

struct set_meta : super_meta {
    symbol *val_type = nullptr;
};

struct function_meta : super_meta {
    symbol                     *belong_class   = nullptr;
    bool                        is_static      = false;
    std::vector<type_qualifier> arg_qualifiers ;
    std::vector<symbol *>       arg_types      ;
    type_qualifier              ret_qualifier  = type_qualifier::value;
    symbol                     *ret_type       = nullptr;
    base_function::ptr          function       ;
    const char *                annotation     = nullptr;
};

struct class_meta : super_meta {
    std::map<symbol *, function_meta *> function_map;
    std::vector<function_meta *>        function_seq;
    std::map<symbol *, const_meta *>    const_map   ;
    std::vector<const_meta *>           const_seq   ;
};

void commit_meta(type_meta *meta);

//query:
//

const type_meta *find_type(const char *name);
const std::vector<type_meta *> &all_types();

} //end reflect.
