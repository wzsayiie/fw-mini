#pragma once

#include "reflect.h"

//object:

declare_reflectable_class(MObject)
class dash_exportable MObject : public reflect::extends<MObject, reflect::injectable> {
};

template<class Class, class Super> class dash_exportable MExtends : public reflect::extends<Class, Super> {
public:
    typedef MExtends<Class, Super> upper;
};

//definition:

#define minikit_class(Class, Super)         \
/**/    declare_reflectable_class(Class)    \
/**/    class dash_exportable Class : public MExtends<Class, Super>

#define minikit_enum(Enum)              \
/**/    declare_reflectable_enum(Enum)  \
/**/    enum class Enum

//builtin types:

template<class F>          using MFunction = reflect::function<F>;
template<class V>          using MVector   = reflect::vector<V>;
template<class K, class V> using MMap      = reflect::map<K, V>;
template<class V>          using MSet      = reflect::set<V>;

using MBaseFunction = reflect::base_function;

//host flags:

#define MINIKIT_HOST_CALL
#define MINIKIT_HOST_IMPL
