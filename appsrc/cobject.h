#pragma once

#include "minikit.h"

declare_reflectable_class(CObject)
class d_exportable CObject : public MExtends<CObject, MObject> {
};

template<class Class, class Super> class d_exportable CExtends : public MExtends<Class, Super> {
public:
    typedef CExtends<Class, Super> upper;

public:
    using _middle = MExtends<Class, Super>;
    using _middle::_middle;
};

#define c_class(Class, Super)               \
/**/    declare_reflectable_class(Class)    \
/**/    class d_exportable Class : public CExtends<Class, Super>

#define c_enum(Enum)                    \
/**/    declare_reflectable_enum(Enum)  \
/**/    enum class Enum
