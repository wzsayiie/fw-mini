#pragma once

#include "reflect.h"

//object:

declare_reflectable_class(MObject)
class dash_exportable MObject : public reflect::extends<MObject, reflect::injectable> {
};

template<class Class, class Super> class dash_exportable MExtends : public reflect::extends<Class, Super> {
public:
    using   reflect::extends<Class, Super>::extends;
    typedef MExtends<Class, Super> base;
};

#define m_class(Class, Super)               \
/**/    declare_reflectable_class(Class)    \
/**/    class dash_exportable Class : public MExtends<Class, Super>

//builtin types:

template<class K, class V> using MMap = reflect::map<K, V>;

template<class V> using MVector   = reflect::vector<V>;
template<class V> using MSet      = reflect::set<V>;
template<class F> using MFunction = reflect::function<F>;

//macro:

//flag for needing host to implement and call.
#define M_HOST_IMPLEMENT_CLASS
#define M_HOST_CALL_FUNCTION
