#pragma once

#include "minikit.h"

declare_reflectable_class(CObject)
class CObject : public MExtends<CObject, MObject> {
};

template<class Class, class Super> struct CExtends : MExtends<Class, Super> {
};
