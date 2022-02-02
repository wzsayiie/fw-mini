#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "boot.h"
#include "mconvention.h"
#include "mdebug.h"

//------------------------------------------------------------------------------
//basic types:

template<> struct MTypeIdOf<void            > { static const MTypeId Value = MHashId("void"  ); };
template<> struct MTypeIdOf<bool            > { static const MTypeId Value = MHashId("bool"  ); };
template<> struct MTypeIdOf<int             > { static const MTypeId Value = MHashId("int"   ); };
template<> struct MTypeIdOf<int64_t         > { static const MTypeId Value = MHashId("int64" ); };
template<> struct MTypeIdOf<float           > { static const MTypeId Value = MHashId("float" ); };
template<> struct MTypeIdOf<double          > { static const MTypeId Value = MHashId("double"); };
template<> struct MTypeIdOf<uint8_t        *> { static const MTypeId Value = MHashId("intptr"); };
template<> struct MTypeIdOf<const uint8_t  *> { static const MTypeId Value = MHashId("intptr"); };
template<> struct MTypeIdOf<char           *> { static const MTypeId Value = MHashId("u8ptr" ); };
template<> struct MTypeIdOf<const char     *> { static const MTypeId Value = MHashId("u8ptr" ); };
template<> struct MTypeIdOf<char16_t       *> { static const MTypeId Value = MHashId("u16ptr"); };
template<> struct MTypeIdOf<const char16_t *> { static const MTypeId Value = MHashId("u16ptr"); };

//------------------------------------------------------------------------------
//MObject:

typedef std::function<void (class MObject *object, int refCount)> _MRefObserver;

template<> struct MTypeIdOf<class MObject *> {
    static const MTypeId Value =  MHashId("MObject");
};

def_class(MObject) {

public:
    void _retain ();
    void _release();
    
    virtual MTypeId _typeId() = 0;

    //for debugging.
    void _setRefObserver(_MRefObserver observer);
    int  _refCount();

protected:
    virtual ~MObject() {}

private:
    _MRefObserver mRefObserver;
    int mRefCount = 1;
};

M_FUNC_EXPORT MObject *MRetain   (MObject *object) M_META(MRetain   , "args:obj");
M_FUNC_EXPORT void     MRelease  (MObject *object) M_META(MRelease  , "args:obj");
M_FUNC_EXPORT MTypeId  MGetTypeId(MObject *object) M_META(MGetTypeId, "args:obj");

//------------------------------------------------------------------------------
//MBool & MInt & MFloat & MPtr:

m_class(MBool  );
m_class(MInt   );
m_class(MInt64 );
m_class(MFloat );
m_class(MDouble);
m_class(MPtr   );

M_FUNC_EXPORT MBool   *MBoolCreate  (bool     value) M_META(MBoolCreate  , "args:value");
M_FUNC_EXPORT MInt    *MIntCreate   (int      value) M_META(MIntCreate   , "args:value");
M_FUNC_EXPORT MInt64  *MInt64Create (int64_t  value) M_META(MInt64Create , "args:value");
M_FUNC_EXPORT MFloat  *MFloatCreate (float    value) M_META(MFloatCreate , "args:value");
M_FUNC_EXPORT MDouble *MDoubleCreate(double   value) M_META(MDoubleCreate, "args:value");
M_FUNC_EXPORT MPtr    *MPtrCreate   (uint8_t *value) M_META(MPtrCreate   , "args:value");

M_FUNC_EXPORT bool     MBoolValue  (MBool   *object) M_META(MBoolValue  , "args:obj");
M_FUNC_EXPORT int      MIntValue   (MInt    *object) M_META(MIntValue   , "args:obj");
M_FUNC_EXPORT int64_t  MInt64Value (MInt64  *object) M_META(MInt64Value , "args:obj");
M_FUNC_EXPORT float    MFloatValue (MFloat  *object) M_META(MFloatValue , "args:obj");
M_FUNC_EXPORT double   MDoubleValue(MDouble *object) M_META(MDoubleValue, "args:obj");
M_FUNC_EXPORT uint8_t *MPtrValue   (MPtr    *object) M_META(MPtrValue   , "args:obj");

//------------------------------------------------------------------------------
//MString:

m_class(MString);

M_FUNC_EXPORT MString *MStringCreateU8 (const char     *chars) M_META(MStringCreateU8 , "args:value");
M_FUNC_EXPORT MString *MStringCreateU16(const char16_t *chars) M_META(MStringCreateU16, "args:value");

M_FUNC_EXPORT MString *MStringCopyU8 (const char     *begin, int size) M_META(MStringCopyU8 , "args:begin,size");
M_FUNC_EXPORT MString *MStringCopyU16(const char16_t *begin, int size) M_META(MStringCopyU16, "args:begin,size");

M_FUNC_EXPORT const char     *MStringU8Chars (MString *string) M_META(MStringU8Chars , "args:str");
M_FUNC_EXPORT const char16_t *MStringU16Chars(MString *string) M_META(MStringU16Chars, "args:str");

M_FUNC_EXPORT int MStringU8Size (MString *string) M_META(MStringU8Size , "args:str");
M_FUNC_EXPORT int MStringU16Size(MString *string) M_META(MStringU16Size, "args:str");

//------------------------------------------------------------------------------
//MLambda:

typedef void (*MLambdaFunc)(MObject *load);

m_class(MLambda);

M_FUNC_EXPORT MLambda *MLambdaCreate(MLambdaFunc func, MObject *load);
M_FUNC_EXPORT void MLambdaCall(MLambda *lambda) M_META(MLambdaCall, "args:lambda");

//------------------------------------------------------------------------------
//MData:

m_class(MData);

M_FUNC_EXPORT MData   *MDataCopy  (const uint8_t *bytes, int size)              M_META(MDataCopy  , "args:ptr,len");
M_FUNC_EXPORT MData   *MDataCreate(int    size)                                 M_META(MDataCreate, "args:len");
M_FUNC_EXPORT void     MDataAppend(MData *data, const uint8_t *bytes, int size) M_META(MDataAppend, "args:dat,ptr,len");
M_FUNC_EXPORT uint8_t *MDataBytes (MData *data)                                 M_META(MDataBytes , "args:dat");
M_FUNC_EXPORT uint8_t *MDataEnd   (MData *data)                                 M_META(MDataEnd   , "args:dat");
M_FUNC_EXPORT int      MDataSize  (MData *data)                                 M_META(MDataSize  , "args:dat");

//------------------------------------------------------------------------------
//MArray:

m_class(MArray);

M_FUNC_EXPORT MArray  *MArrayCreate()                             M_META(MArrayCreate);
M_FUNC_EXPORT void     MArrayAppend(MArray *array, MObject *item) M_META(MArrayAppend, "args:array,item");
M_FUNC_EXPORT int      MArrayLength(MArray *array)                M_META(MArrayLength, "args:array");
M_FUNC_EXPORT MObject *MArrayItem  (MArray *array, int index)     M_META(MArrayItem  , "args:array,index");

//------------------------------------------------------------------------------
//MUnknown:

m_class(MUnknown);

//------------------------------------------------------------------------------
//smart pointer:

struct _MMakeSharedHelper {
    template<typename T> std::shared_ptr<T> operator<<(T *object) {
        if (object) {
            MRetain(object);
            return std::shared_ptr<T>(object, MRelease);
        } else {
            return std::shared_ptr<T>();
        }
    }
};
#define m_make_shared _MMakeSharedHelper()<<

struct _MAutoReleaseHelper {
    template<typename T> std::shared_ptr<T> operator<<(T *object) {
        if (object) {
            return std::shared_ptr<T>(object, MRelease);
        } else {
            return std::shared_ptr<T>();
        }
    }
};
#define m_auto_release _MAutoReleaseHelper()<<

//------------------------------------------------------------------------------
//lambda cast:

struct _MLambdaCastHelper {
    MLambdaRef operator<<(std::function<void ()> func);
};
#define m_cast_lambda _MLambdaCastHelper()<<
