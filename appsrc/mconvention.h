#pragma once

#include "menviron.h"

//------------------------------------------------------------------------------
//type definition:

//type id and hash function:
typedef int MTypeId;

template<typename T> struct MTypeIdOf;

constexpr int _MHashFactor(char chr) {
    if ('A' <= chr && chr <= 'Z') { return chr - 'A' + 1; }
    if ('a' <= chr && chr <= 'z') { return chr - 'a' + 1; }
    
    return chr ? 27 : 0;
}
constexpr int _MHashId(const char *name) {
    return (
        (_MHashFactor(name[0])      ) |
        (_MHashFactor(name[1]) <<  5) |
        (_MHashFactor(name[2]) << 10) |
        (_MHashFactor(name[3]) << 15) |
        (_MHashFactor(name[4]) << 20) |
        (_MHashFactor(name[5]) << 25)
    );
}
#define MHashId(name) _MHashId(name "\0\0\0\0\0")

//to define a class with type id.
#define m_class(name)                                       \
/**/    template<> struct MTypeIdOf<class name *> {         \
/**/        static const MTypeId Value = MHashId(#name);    \
/**/    };                                                  \
/**/                                                        \
/**/    typedef std::shared_ptr<class name> name##Ref;      \
/**/                                                        \
/**/    class name : public MObject {                       \
/**/        MTypeId typeId() override {                     \
/**/            return MTypeIdOf<name *>::Value;            \
/**/        }                                               \
/**/    }

//functions that need to be called by the host.
#define M_FUNC_HOST extern "C"

//exported functions.
#if DASH_COMPILER_MSC
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
