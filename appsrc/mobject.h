#pragma once

#include "reflect.h"

//object:

declare_reflectable_class(MObject)
class MObject : public reflect::extends<MObject, reflect::injectable> {
};

template<class Class, class Super> struct MExtends : reflect::extends<Class, Super> {
};

//builtin types:

template<class K, class V> using MMap = reflect::map<K, V>;

template<class V> using MVector   = reflect::vector<V>;
template<class V> using MSet      = reflect::set<V>;
template<class F> using MFunction = reflect::function<F>;

//macro:

//flag for needing host to implement and call.
#define M_HOST_IMPLEMENT_CLASS
#define M_HOST_CALL_FUNCTION
