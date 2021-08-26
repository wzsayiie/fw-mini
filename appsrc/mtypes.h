#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "mconfig.h"

//------------------------------------------------------------------------------
//c++ idioms:

#define m_static_object(name, ...) static __VA_ARGS__ &name { static auto a = new __VA_ARGS__; return *a; }

//------------------------------------------------------------------------------
//MObject:

typedef std::function<void (class MObject *object, int refCount)> _MRefObserver;

class MObject {

public:
    void _retain ();
    void _release();
    
    virtual MType _type() = 0;

    //for debugging.
    void _setRefObserver(_MRefObserver observer);
    int  _refCount();

protected:
    virtual ~MObject() {}

private:
    _MRefObserver mRefObserver;
    int mRefCount = 1;
};

MFUNC_EXPORT MObject *MRetain (MObject *object) MFUNC_META(MRetain );
MFUNC_EXPORT void     MRelease(MObject *object) MFUNC_META(MRelease);
MFUNC_EXPORT MType    MGetType(MObject *object) MFUNC_META(MGetType);

//------------------------------------------------------------------------------
//MBool & MInt & MFloat & MPointer:

class MBool    : public MObject {};
class MInt     : public MObject {};
class MFloat   : public MObject {};
class MPointer : public MObject {};

MFUNC_EXPORT MBool    *MBoolCreate   (bool     value) MFUNC_META(MBoolCreate   );
MFUNC_EXPORT MInt     *MIntCreate    (int      value) MFUNC_META(MIntCreate    );
MFUNC_EXPORT MFloat   *MFloatCreate  (float    value) MFUNC_META(MFloatCreate  );
MFUNC_EXPORT MPointer *MPointerCreate(uint8_t *value) MFUNC_META(MPointerCreate);

MFUNC_EXPORT bool     MBoolValue   (MBool    *object) MFUNC_META(MBoolValue   );
MFUNC_EXPORT int      MIntValue    (MInt     *object) MFUNC_META(MIntValue    );
MFUNC_EXPORT float    MFloatValue  (MFloat   *object) MFUNC_META(MFloatValue  );
MFUNC_EXPORT uint8_t *MPointerValue(MPointer *object) MFUNC_META(MPointerValue);

//------------------------------------------------------------------------------
//MString:

class MString : public MObject {};

MFUNC_EXPORT MString *MStringCreateU8 (const char     *chars) MFUNC_META(MStringCreateU8 );
MFUNC_EXPORT MString *MStringCreateU16(const char16_t *chars) MFUNC_META(MStringCreateU16);

MFUNC_EXPORT const char     *MStringU8Chars (MString *string) MFUNC_META(MStringU8Chars );
MFUNC_EXPORT const char16_t *MStringU16Chars(MString *string) MFUNC_META(MStringU16Chars);

MFUNC_EXPORT int MStringU8Size (MString *string) MFUNC_META(MStringU8Size );
MFUNC_EXPORT int MStringU16Size(MString *string) MFUNC_META(MStringU16Size);

//------------------------------------------------------------------------------
//MLambda:

typedef void (*MLambdaFunc)(MObject *load);

class MLambda : public MObject {};

MFUNC_EXPORT MLambda *MLambdaCreate(MLambdaFunc func, MObject *load);
MFUNC_EXPORT void MLambdaCall(MLambda *lambda) MFUNC_META(MLambdaCall);

//------------------------------------------------------------------------------
//MData:

class MData : public MObject {};

MFUNC_EXPORT MData *MDataCreate(const uint8_t *bytes, int size)              MFUNC_META(MDataCreate);
MFUNC_EXPORT void   MDataAppend(MData *data, const uint8_t *bytes, int size) MFUNC_META(MDataAppend);
MFUNC_EXPORT int    MDataSize  (MData *data)                                 MFUNC_META(MDataSize  );

MFUNC_EXPORT const uint8_t *MDataBytes(MData *data) MFUNC_META(MDataBytes);

//------------------------------------------------------------------------------
//MArray:

class MArray : public MObject {};

MFUNC_EXPORT MArray  *MArrayCreate()                             MFUNC_META(MArrayCreate);
MFUNC_EXPORT void     MArrayAppend(MArray *array, MObject *item) MFUNC_META(MArrayAppend);
MFUNC_EXPORT int      MArrayLength(MArray *array)                MFUNC_META(MArrayLength);
MFUNC_EXPORT MObject *MArrayItem  (MArray *array, int index)     MFUNC_META(MArrayItem  );

//------------------------------------------------------------------------------
//MImage:

class MImage : public MObject {};

MFUNC_EXPORT MImage  *MImageCreate (MObject *load ) MFUNC_META(MImageCreate );
MFUNC_EXPORT MObject *MImageGetLoad(MImage  *image) MFUNC_META(MImageGetLoad);

//------------------------------------------------------------------------------
//MSpecial:

class MSpecial : public MObject {
    
public:
    MType _type() override;
};

//------------------------------------------------------------------------------
//smart pointer:

typedef std::shared_ptr<MObject> MObjectRef;
typedef std::shared_ptr<MBool  > MBoolRef  ;
typedef std::shared_ptr<MInt   > MIntRef   ;
typedef std::shared_ptr<MFloat > MFloatRef ;
typedef std::shared_ptr<MString> MStringRef;
typedef std::shared_ptr<MLambda> MLambdaRef;
typedef std::shared_ptr<MData  > MDataRef  ;
typedef std::shared_ptr<MArray > MArrayRef ;
typedef std::shared_ptr<MImage > MImageRef ;

class _MMakeSharedHelper {

public:
    MObjectRef operator<<(MObject *a) { return makeShared(a); }
    MBoolRef   operator<<(MBool   *a) { return makeShared(a); }
    MIntRef    operator<<(MInt    *a) { return makeShared(a); }
    MFloatRef  operator<<(MFloat  *a) { return makeShared(a); }
    MStringRef operator<<(MString *a) { return makeShared(a); }
    MLambdaRef operator<<(MLambda *a) { return makeShared(a); }
    MDataRef   operator<<(MData   *a) { return makeShared(a); }
    MArrayRef  operator<<(MArray  *a) { return makeShared(a); }
    MImageRef  operator<<(MImage  *a) { return makeShared(a); }

private:
    template<typename T> std::shared_ptr<T> makeShared(T *object) {
        MRetain(object);
        return std::shared_ptr<T>(object, MRelease);
    }
};

#define m_make_shared _MMakeSharedHelper()<<

class _MAutoReleaseHelper {

public:
    MObjectRef operator<<(MObject *a) { return autoRelease(a); }
    MBoolRef   operator<<(MBool   *a) { return autoRelease(a); }
    MIntRef    operator<<(MInt    *a) { return autoRelease(a); }
    MFloatRef  operator<<(MFloat  *a) { return autoRelease(a); }
    MStringRef operator<<(MString *a) { return autoRelease(a); }
    MLambdaRef operator<<(MLambda *a) { return autoRelease(a); }
    MDataRef   operator<<(MData   *a) { return autoRelease(a); }
    MArrayRef  operator<<(MArray  *a) { return autoRelease(a); }
    MImageRef  operator<<(MImage  *a) { return autoRelease(a); }

private:
    template<typename T> std::shared_ptr<T> autoRelease(T *object) {
        return std::shared_ptr<T>(object, MRelease);
    }
};

#define m_auto_release _MAutoReleaseHelper()<<

//------------------------------------------------------------------------------
//lambda cast:

struct _MLambdaCastHelper {
    MLambdaRef operator<<(std::function<void ()> func);
};

#define m_cast_lambda _MLambdaCastHelper()<<
