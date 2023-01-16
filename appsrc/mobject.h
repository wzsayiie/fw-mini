#pragma once

#include "rdeclare.h"
#include "rinjectable.h"

declare_reflectable_class(MObject)
class d_exportable MObject : public reflect::extends<MObject, reflect::injectable> {
};

#define m_class(Class, Base)                                            \
/**/    declare_reflectable_class(Class)                                \
/**/    class d_exportable Class : public reflect::extends<Class, Base>

#define m_enum(Enum)                    \
/**/    declare_reflectable_enum(Enum)  \
/**/    enum class Enum

#define M_HOST_CALL
#define M_HOST_IMPL
