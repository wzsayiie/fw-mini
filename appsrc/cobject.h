#pragma once

#include "minikit.h"

declare_reflectable_class(CObject)
class dash_exportable CObject : public MExtends<CObject, MObject> {
};

template<class Class, class Super> class dash_exportable CExtends : public MExtends<Class, Super> {
public:
    using   MExtends<Class, Super>::MExtends;
    typedef CExtends<Class, Super> base;
};

#define c_class(Class, Super)               \
/**/    declare_reflectable_class(Class)    \
/**/    class dash_exportable Class : public CExtends<Class, Super>
