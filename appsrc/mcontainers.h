#pragma once

#include "mobject.h"

//function:
template<class F> using MFunction = reflect::function<F>;

using MGenericFunction = reflect::generic_function;

#define MF reflect::make_function

//element types.
m_enum(MDataType) {
    Void   = (int)reflect::data_type::is_void  ,
    Bool   = (int)reflect::data_type::is_bool  ,
    Byte   = (int)reflect::data_type::is_byte  ,
    Int    = (int)reflect::data_type::is_int   ,
    Int64  = (int)reflect::data_type::is_int64 ,
    Float  = (int)reflect::data_type::is_float ,
    Double = (int)reflect::data_type::is_double,
    String = (int)reflect::data_type::is_string,
    Object = (int)reflect::data_type::is_object,
};

//containers:
template<class V>          using MVector = reflect::vector<V>;
template<class K, class V> using MMap    = reflect::map<K, V>;
template<class V>          using MSet    = reflect::set<V>;

using MGenericVector = reflect::generic_vector;
using MGenericMap    = reflect::generic_map   ;
using MGenericSet    = reflect::generic_set   ;
