#pragma once

#include <cstdint>
#include <memory>
#include "mconfig.h"

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

MFUNC_BASE void  MRetain (MObject *object);
MFUNC_BASE void  MRelease(MObject *object);
MFUNC_BASE MType MGetType(MObject *object);

//------------------------------------------------------------------------------
//MBool & MInt & MFloat:

class MBool : public MObject {};
class MInt  : public MObject {};
class MFloat: public MObject {};

MFUNC_BASE MBool  *MBoolCreate (bool  value);
MFUNC_BASE MInt   *MIntCreate  (int   value);
MFUNC_BASE MFloat *MFloatCreate(float value);

MFUNC_BASE bool  MBoolValue (MBool  *object);
MFUNC_BASE int   MIntValue  (MInt   *object);
MFUNC_BASE float MFloatValue(MFloat *object);

//------------------------------------------------------------------------------
//MString:

class MString : public MObject {};

MFUNC_BASE MString *MStringCreateU8 (const char     *chars);
MFUNC_BASE MString *MStringCreateU16(const char16_t *chars);

MFUNC_BASE const char     *MStringU8Chars (MString *string);
MFUNC_BASE const char16_t *MStringU16Chars(MString *string);

MFUNC_BASE int MStringU8Size (MString *string);
MFUNC_BASE int MStringU16Size(MString *string);

//------------------------------------------------------------------------------
//MLambda:

typedef void (*MLambdaFunc)(MObject *load);

class MLambda : public MObject {};

MFUNC_BASE MLambda *MLambdaCreate(MLambdaFunc func, MObject *load);
MFUNC_BASE void MLambdaCall(MLambda *lambda);

//------------------------------------------------------------------------------
//MData:

class MData : public MObject {};

MFUNC_BASE MData *MDataCreate(const uint8_t *bytes, int size);
MFUNC_BASE void   MDataAppend(MData *data, const uint8_t *bytes, int size);
MFUNC_BASE int    MDataSize  (MData *data);

MFUNC_BASE const uint8_t *MDataBytes(MData *data);

//------------------------------------------------------------------------------
//MArray:

class MArray : public MObject {};

MFUNC_BASE MArray  *MArrayCreate();
MFUNC_BASE void     MArrayAppend(MArray *array, MObject *item);
MFUNC_BASE int      MArrayLength(MArray *array);
MFUNC_BASE MObject *MArrayItem  (MArray *array, int index);

//------------------------------------------------------------------------------
//MImage:

typedef void (*MImageDispose)(int managedId);

class MImage : public MObject {};

MFUNC_BASE MImage  *MImageCreate (MObject *load );
MFUNC_BASE MObject *MImageGetLoad(MImage  *image);

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
