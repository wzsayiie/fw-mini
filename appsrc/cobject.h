#pragma once

#include "minikit.h"

declare_reflectable_class(CObject)
class CObject : public MExtends<CObject, MObject> {
};

template<class Class, class Super> class CExtends : public MExtends<Class, Super> {
public:
    using   MExtends<Class, Super>::MExtends;
    typedef CExtends<Class, Super> base;
};

#define c_class(Class, Super)                       \
/**/    declare_reflectable_class(Class)            \
/**/    class Class : public CExtends<Class, Super>
