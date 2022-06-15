#pragma once

#include "rfunction.h"

namespace reflect {

//type categories and qualifiers:

enum class category {
    is_void    ,    //void.
    is_bool    ,    //bool.
    is_char    ,    //char.
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

enum class qualifier {
    value       ,   //value.
    const_ptr   ,   //const value *.
    ptr         ,   //value *.
    const_ref   ,   //const value &.
    ref         ,   //value &.
    const_shared,   //const std::shared_ptr<value> &.
    shared      ,   //std::shared_ptr<value>.
};

#define _Q_V(VALUE) static constexpr qualifier value = VALUE

template<class  > struct qualifier_of                             { _Q_V(qualifier::value       ); };
template<class T> struct qualifier_of<const T *                 > { _Q_V(qualifier::const_ptr   ); };
template<class T> struct qualifier_of<T *                       > { _Q_V(qualifier::ptr         ); };
template<class T> struct qualifier_of<const T &                 > { _Q_V(qualifier::const_ref   ); };
template<class T> struct qualifier_of<T &                       > { _Q_V(qualifier::ref         ); };
template<class T> struct qualifier_of<const std::shared_ptr<T> &> { _Q_V(qualifier::const_shared); };
template<class T> struct qualifier_of<std::shared_ptr<T> &      > { _Q_V(qualifier::shared      ); };

#undef  _Q_V

//variable:

struct variable {
    symbol type ;
    any    value;
};

//metas:

struct type_meta : dash::virtual_object {
    category cate;
};

struct function_meta : type_meta {
    std::vector<qualifier> arg_quals;
    std::vector<symbol>    arg_types;
    qualifier              ret_qual ;
    symbol                 ret_type ;
    std::string            note     ;
};

struct vector_meta : type_meta {
    symbol value_type;
};

struct map_meta : type_meta {
    symbol key_type  ;
    symbol value_type;
};

struct set_meta : type_meta {
    symbol value_type;
};

struct class_meta : type_meta {
    symbol base_type;
    bool   abstract ;
    
    std::map<std::string, variable> static_variables;
    std::map<std::string, variable> static_functions;
    std::map<std::string, variable> inst_functions;
};

struct enum_meta : type_meta {
    std::map<std::string, variable> values;
};

//storage:

type_meta *commit_type_meta(const symbol &sym, category cate);

void commit_variable             (/* global scope */ const std::string &name, const symbol &type, const any &value);
void commit_function             (/* global scope */ const std::string &name, const symbol &type, const any &value);
void commit_class                (/* global scope */ const std::string &name, const symbol &type);
void commit_enum                 (/* global scope */ const std::string &name, const symbol &type);
void commit_class_static_variable(const symbol &cls, const std::string &name, const symbol &type, const any &value);
void commit_class_static_function(const symbol &cls, const std::string &name, const symbol &type, const any &value);
void commit_class_inst_function  (const symbol &cls, const std::string &name, const symbol &type, const any &value);
void commit_enum_value           (const symbol &enu, const std::string &name, const symbol &type, const any &value);

dash_exportable const std::map<symbol, type_meta *> *type_metas();

dash_exportable const type_meta *query_type_meta(const symbol &sym, category cate);
dash_exportable const type_meta *query_type_meta(const symbol &sym);

dash_exportable const std::map<std::string, variable> *variables();
dash_exportable const std::map<std::string, variable> *functions();
dash_exportable const std::map<std::string, variable> *classes  ();
dash_exportable const std::map<std::string, variable> *enums    ();

} //end reflect.
