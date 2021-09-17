#pragma once

#include <memory>
#include "menviron.h"

//------------------------------------------------------------------------------
//type definition:

#define MEnumId(n) (((int)n[0]) | ((int)n[1] << 8) | ((int)n[2] << 16))

typedef int MTypeId;

template<typename T> struct MTypeIdOf;

template<> struct MTypeIdOf<void            > { static const MTypeId Value = MEnumId("vid"); };
template<> struct MTypeIdOf<bool            > { static const MTypeId Value = MEnumId("bol"); };
template<> struct MTypeIdOf<int             > { static const MTypeId Value = MEnumId("int"); };
template<> struct MTypeIdOf<float           > { static const MTypeId Value = MEnumId("flt"); };
template<> struct MTypeIdOf<uint8_t        *> { static const MTypeId Value = MEnumId("ptr"); };
template<> struct MTypeIdOf<const uint8_t  *> { static const MTypeId Value = MEnumId("ptr"); };
template<> struct MTypeIdOf<char           *> { static const MTypeId Value = MEnumId("s08"); };
template<> struct MTypeIdOf<const char     *> { static const MTypeId Value = MEnumId("s08"); };
template<> struct MTypeIdOf<char16_t       *> { static const MTypeId Value = MEnumId("s16"); };
template<> struct MTypeIdOf<const char16_t *> { static const MTypeId Value = MEnumId("s16"); };

//to define a class with type id.
#define m_class(name, id)                               \
/**/    template<> struct MTypeIdOf<class name *> {     \
/**/        static const MTypeId Value = MEnumId(id);   \
/**/    };                                              \
/**/                                                    \
/**/    typedef std::shared_ptr<class name> name##Ref;  \
/**/                                                    \
/**/    struct _##name##_Middler : MObject {            \
/**/        MTypeId _typeId() override {                \
/**/            return MEnumId(id);                     \
/**/        }                                           \
/**/    };                                              \
/**/                                                    \
/**/    class name : public _##name##_Middler

//to create a inner static object.
#define m_static_object(name, ...)              \
/**/    static __VA_ARGS__ &name {              \
/**/        static auto a = new __VA_ARGS__;    \
/**/        return *a;                          \
/**/    }

//------------------------------------------------------------------------------
//function meta information:

const int MFuncMaxArgCount = 4;

struct _MFuncMeta {
    void   *address   = nullptr;
    MTypeId retTypeId = 0;
    bool    retRetain = false;
    int     argCount  = 0;

    MTypeId argTypeIds[MFuncMaxArgCount] = {0};
};

//------------------------------------------------------------------------------
//macros for function descritpion:

//functions that need to be called by the host.
#define MFUNC_HOST extern "C"

//exported functions.
#if MCPL_CL
    #define MFUNC_EXPORT extern "C" __declspec(dllexport)
#else
    #define MFUNC_EXPORT extern "C" __attribute((visibility("default")))
#endif

//functions recorded meta data.
#ifndef MFUNC_META
    #define MFUNC_META(name)
#endif
