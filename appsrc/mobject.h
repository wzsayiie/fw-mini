#pragma once

#include "reflect.h"

//object:

declare_reflectable_class(MObject)
class d_exportable MObject : public reflect::extends<MObject, reflect::injectable> {
};

//definition:

#define m_class(Class, Super)                                           \
/**/    declare_reflectable_class(Class)                                \
/**/    class d_exportable Class : public reflect::extends<Class, Super>

#define m_enum(Enum)                    \
/**/    declare_reflectable_enum(Enum)  \
/**/    enum class Enum

//builtin types:

template<class F>          using MFunction = reflect::function<F>;
template<class V>          using MVector   = reflect::vector<V>;
template<class K, class V> using MMap      = reflect::map<K, V>;
template<class V>          using MSet      = reflect::set<V>;

using MBaseFunction = reflect::base_function;

#define MF reflect::make_function

//host flags:

#define M_HOST_CALL
#define M_HOST_IMPL
