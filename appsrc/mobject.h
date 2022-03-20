#pragma once

#include "reflect.h"

//object:
//

declare_reflectable_class(MObject)
class MObject : public reflect::extends<MObject, reflect::injectable> {
};

template<class Class, class Super> struct MExtends : reflect::extends<Class, Super> {
};

//builtin types:
//

#define MFunction reflect::function
#define MVector   reflect::vector
#define MMap      reflect::map
#define MSet      reflect::set

declare_reflectable_class(MFunction<void ()>)

//macro:
//

//flag for needing host to implement and call.
#define M_HOST_IMPLEMENT_CLASS
#define M_HOST_CALL_FUNCTION
