#pragma once

#include "reflect.h"

declare_reflectable_class(MObject)
class d_exportable MObject : public reflect::extends<MObject, reflect::injectable> {
};

#define m_class(Class, Super)                                           \
/**/    declare_reflectable_class(Class)                                \
/**/    class d_exportable Class : public reflect::extends<Class, Super>

#define m_enum(Enum)                    \
/**/    declare_reflectable_enum(Enum)  \
/**/    enum class Enum

#define M_HOST_CALL
#define M_HOST_IMPL
