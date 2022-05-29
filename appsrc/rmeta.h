#pragma once

#include "rfunction.h"

namespace reflect {

//variable:

struct variable {
    symbol type ;
    any    value;
};

//type categories:

enum class type_category {
    is_void    ,    //void.
    is_bool    ,    //bool.
    is_byte    ,    //uint8_t.
    is_int     ,    //int.
    is_int64   ,    //int64_t.
    is_float   ,    //float.
    is_double  ,    //double.
    is_string  ,    //std::string.
    
    is_function,    //reflect::function<ret (args...)>.
    is_vector  ,    //reflect::vector<value>.
    is_map     ,    //reflect::map<key, value>.
    is_set     ,    //reflect::set<value>.

    is_class   ,    //reflect::object (exclude function, vector, map and set).
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

//metas:

struct type_meta : dash::virtual_object {
    type_category category;
};

struct function_meta : type_meta {
    std::vector<type_qualifier> arg_qualifiers ;
    std::vector<symbol>         arg_types      ;
    type_qualifier              ret_qualifier  ;
    symbol                      ret_type       ;
};

struct vector_meta : type_meta {
    symbol val_type;
};

struct map_meta : type_meta {
    symbol key_type;
    symbol val_type;
};

struct set_meta : type_meta {
    symbol val_type;
};

struct class_meta : type_meta {
    std::map<std::string, variable> cls_variables;
    std::map<std::string, variable> cls_functions;
    std::map<std::string, variable> obj_functions;
};

struct enum_meta : type_meta {
    std::map<std::string, variable> values;
};

//storage:

void commit_type_meta(const symbol &sym, type_meta *meta);

void commit_variable       (/* global scope */ const std::string &name, const variable &value);
void commit_function       (/* global scope */ const std::string &name, const variable &value);
void commit_class_variable (const symbol &cls, const std::string &name, const variable &value);
void commit_class_function (const symbol &cls, const std::string &name, const variable &value);
void commit_object_function(const symbol &cls, const std::string &name, const variable &value);
void commit_enum_value     (const symbol &enu, const std::string &name, const variable &value);

dash_exportable const type_meta *query_type_meta(const symbol &sym);

dash_exportable const std::map<std::string, variable> *variables();
dash_exportable const std::map<std::string, variable> *functions();
dash_exportable const std::map<std::string, variable> *classes  ();
dash_exportable const std::map<std::string, variable> *enums    ();

} //end reflect.
