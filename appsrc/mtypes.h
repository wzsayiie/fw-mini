#pragma once

#include "mconfig.h"

//------------------------------------------------------------------------------
//common types of the standard library:

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

//------------------------------------------------------------------------------
//type enumeration:

#define MEnumId(n) (((int)n[0]) | ((int)n[1] << 1) | ((int)n[2] << 2))

typedef int MType;

const MType MType_Void    = MEnumId("vid");     //void.
const MType MType_Bool    = MEnumId("bol");     //bool.
const MType MType_Int     = MEnumId("int");     //int.
const MType MType_Float   = MEnumId("flt");     //float.
const MType MType_C8Ptr   = MEnumId("p08");     //char *, uint8_t *.
const MType MType_C16Ptr  = MEnumId("p16");     //char16_t *.
const MType MType_MObject = MEnumId("Obj");     //MObject.
const MType MType_MBool   = MEnumId("Bol");     //MBool.
const MType MType_MInt    = MEnumId("Int");     //MInt.
const MType MType_MFloat  = MEnumId("Flt");     //MFloat.
const MType MType_MString = MEnumId("Str");     //MString.
const MType MType_MLambda = MEnumId("Lmd");     //MLamdba.
const MType MType_MData   = MEnumId("Dat");     //MData.
const MType MType_MArray  = MEnumId("Arr");     //MArray.
const MType MType_MImage  = MEnumId("Img");     //MImage.

//used to present types related to the os.
const MType MType_MNative = MEnumId("Ntv");

//------------------------------------------------------------------------------
//MObject:

class MObject {

public:
    void _retain ();
    void _release();
    
    virtual MType _type() = 0;

protected:
    virtual ~MObject() {}

private:
    int mRefCount = 1;
};

MFUNC_EXPORT void  MRetain (MObject *object);
MFUNC_EXPORT void  MRelease(MObject *object);
MFUNC_EXPORT MType MGetType(MObject *object);

//------------------------------------------------------------------------------
//MBool & MInt & MFloat:

class MBool : public MObject {};
class MInt  : public MObject {};
class MFloat: public MObject {};

MFUNC_EXPORT MBool  *MBoolCreate (bool  value);
MFUNC_EXPORT MInt   *MIntCreate  (int   value);
MFUNC_EXPORT MFloat *MFloatCreate(float value);

MFUNC_EXPORT bool  MBoolValue (MBool  *object);
MFUNC_EXPORT int   MIntValue  (MInt   *object);
MFUNC_EXPORT float MFloatValue(MFloat *object);

//------------------------------------------------------------------------------
//MString:

class MString : public MObject {};

MFUNC_EXPORT MString *MStringCreateU8 (const char     *chars);
MFUNC_EXPORT MString *MStringCreateU16(const char16_t *chars);

MFUNC_EXPORT const char     *MStringU8Chars (MString *string);
MFUNC_EXPORT const char16_t *MStringU16Chars(MString *string);

MFUNC_EXPORT int MStringU8Size (MString *string);
MFUNC_EXPORT int MStringU16Size(MString *string);

//------------------------------------------------------------------------------
//MLambda:

typedef void (*MLambdaFunc)(MObject *load);

class MLambda : public MObject {};

MFUNC_EXPORT MLambda *MLambdaCreate(MLambdaFunc func, MObject *load);
MFUNC_EXPORT void MLambdaCall(MLambda *lambda);

//------------------------------------------------------------------------------
//MData:

class MData : public MObject {};

MFUNC_EXPORT MData *MDataCreate(const uint8_t *bytes, int size);
MFUNC_EXPORT void   MDataAppend(MData *data, const uint8_t *bytes, int size);
MFUNC_EXPORT int    MDataSize  (MData *data);

MFUNC_EXPORT const uint8_t *MDataBytes(MData *data);

//------------------------------------------------------------------------------
//MArray:

class MArray : public MObject {};

MFUNC_EXPORT MArray  *MArrayCreate();
MFUNC_EXPORT void     MArrayAppend(MArray *array, MObject *item);
MFUNC_EXPORT int      MArrayLength(MArray *array);
MFUNC_EXPORT MObject *MArrayItem  (MArray *array, int index);

//------------------------------------------------------------------------------
//MImage:

class MImage : public MObject {};

MFUNC_EXPORT MImage  *MImageCreate (MObject *load );
MFUNC_EXPORT MObject *MImageGetLoad(MImage  *image);

//------------------------------------------------------------------------------
//Native:

class _MNative : public MObject {
    
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

class _MLambdaCastHelper {
    
public:
    template<typename F> MLambdaRef operator<<(const F &func) {
        
        struct Impl : Intf {
            
            F mFunc;
            
            Impl(const F &func): mFunc(func) {
            }
            
            void call() override {
                mFunc();
            }
        };
        
        return m_auto_release MLambdaCreate(procedure, new Impl(func));
    }
    
private:
    struct Intf : _MNative {
        virtual void call() = 0;
    };
    
    static void procedure(MObject *load) {
        ((Intf *)load)->call();
    }
};

#define m_cast_lambda _MLambdaCastHelper()<<
