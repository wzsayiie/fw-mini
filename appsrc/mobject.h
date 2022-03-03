#pragma once

#include "reflect.h"

//object:
//

declare_reflectable_class(MBaseObject)
class MBaseObject : public reflect::extends<MBaseObject, reflect::object> {
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
