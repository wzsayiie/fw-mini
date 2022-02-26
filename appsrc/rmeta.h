#pragma once

#include "rfunction.h"

namespace reflect {

//type:
//

enum class type_category {
    is_void    ,    //void.
    is_bool    ,    //bool.
    is_int     ,    //int.
    is_int64   ,    //int64_t.
    is_float   ,    //float.
    is_double  ,    //double.
    is_string  ,    //std::string.
    
    is_function,    //reflect::function<ret (args...)>.
    
    is_vector  ,    //reflect::vector<value>.
    is_map     ,    //reflect::map<key, value>.
    is_set     ,    //reflect::set<value>.
    
    is_class   ,    //reflect::object (exclude function, map, set and vector).
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

struct type_meta {
    type_category type = type_category::is_void;
    symbol       *name = nullptr;
};

struct super_meta : type_meta {
    object::ptr (*create)() = nullptr;
};

struct function_meta : super_meta {
    symbol                     *belong_class   = nullptr;
    bool                        is_static      = false;
    std::vector<type_qualifier> arg_qualifiers ;
    std::vector<symbol *>       arg_types      ;
    type_qualifier              ret_qualifier  = type_qualifier::value;
    symbol                     *ret_type       = nullptr;
    base_function::ptr          function       ;
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

struct class_meta : super_meta {
    std::map<symbol *, struct function_meta *> function_map;
    std::vector<struct function_meta *>        function_seq;
};

void commit_meta(type_meta *meta);

//query:
//

const type_meta     *find_type     (const char *name);
const type_meta     *find_basic    (const char *name);
const function_meta *find_function (const char *name);
const super_meta    *find_container(const char *name);
const class_meta    *find_class    (const char *name);

const std::vector<type_meta     *> &basics    ();
const std::vector<function_meta *> &functions ();
const std::vector<super_meta    *> &containers();
const std::vector<class_meta    *> &classes   ();

} // end reflect.
