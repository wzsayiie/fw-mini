#pragma once

#include "menviron.h"

//------------------------------------------------------------------------------
//type definition:

//type id and enumeration value:
typedef int MTypeId;

template<typename T> struct MTypeIdOf;

#define MEnumId(n) (((int)n[0]) | ((int)n[1] << 8) | ((int)n[2] << 16))

//to define a class with type id.
#define m_class(name, id)                               \
/**/    template<> struct MTypeIdOf<class name *> {     \
/**/        static const MTypeId Value = MEnumId(id);   \
/**/    };                                              \
/**/                                                    \
/**/    typedef std::shared_ptr<class name> name##Ref;  \
/**/                                                    \
/**/    class name : public MObject {                   \
/**/        MTypeId _typeId() override {                \
/**/            return MEnumId(id);                     \
/**/        }                                           \
/**/    }

//functions that need to be called by the host.
#define M_FUNC_HOST extern "C"

//exported functions.
#if M_CPL_CL
    #define M_FUNC_EXPORT extern "C" __declspec(dllexport)
#else
    #define M_FUNC_EXPORT extern "C" __attribute((visibility("default")))
#endif

//------------------------------------------------------------------------------
//meta information:

const int MFuncMaxArgCount = 4;

class MString;

struct _MFuncMeta {
    MString *funcName  = nullptr;
    MString *funcNote  = nullptr;
    void    *address   = nullptr;
    MTypeId  retTypeId = 0;
    bool     retRetain = false;
    int      argCount  = 0;

    MTypeId argTypeIds[MFuncMaxArgCount] = {0};
};

struct _MConstMeta {
    MString *constName   = nullptr;
    MString *constNote   = nullptr;
    MTypeId  benchmarkId = 0;
    MString *stringValue = nullptr;
    int      intValue    = 0;
    float    floatValue  = 0;
};

#ifndef M_META
#define M_META(name, ...)
#endif
