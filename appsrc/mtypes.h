#pragma once

#include <cstdint>
#include <memory>

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

extern "C" void  MRetain (MObject *object);
extern "C" void  MRelease(MObject *object);
extern "C" MType MGetType(MObject *object);

//------------------------------------------------------------------------------
//MBool & MInt & MFloat:

class MBool : public MObject {};
class MInt  : public MObject {};
class MFloat: public MObject {};

extern "C" MBool  *MBoolCreate (bool  value);
extern "C" MInt   *MIntCreate  (int   value);
extern "C" MFloat *MFloatCreate(float value);

extern "C" bool  MBoolValue (MBool  *object);
extern "C" int   MIntValue  (MInt   *object);
extern "C" float MFloatValue(MFloat *object);

//------------------------------------------------------------------------------
//MString:

class MString : public MObject {};

extern "C" MString *MStringCreateU8 (const char     *chars);
extern "C" MString *MStringCreateU16(const char16_t *chars);

extern "C" const char     *MStringU8Chars (MString *string);
extern "C" const char16_t *MStringU16Chars(MString *string);

extern "C" int MStringU8Size (MString *string);
extern "C" int MStringU16Size(MString *string);

//------------------------------------------------------------------------------
//MLambda:

typedef void (*MLambdaFunc)(MObject *load, MObject *param);

class MLambda : public MObject {};

extern "C" MLambda *MLambdaCreate(MLambdaFunc func, MObject *load);
extern "C" void MLambdaCall(MLambda *lambda, MObject *param);

//------------------------------------------------------------------------------
//MData:

class MData : public MObject {};

extern "C" MData *MDataCreate(const uint8_t *bytes, int size);
extern "C" void   MDataAppend(MData *data, const uint8_t *bytes, int size);
extern "C" int    MDataSize  (MData *data);

extern "C" const uint8_t *MDataBytes(MData *data);

//------------------------------------------------------------------------------
//MArray:

class MArray : public MObject {};

extern "C" MArray  *MArrayCreate();
extern "C" void     MArrayAppend(MArray *array, MObject *item);
extern "C" int      MArrayLength(MArray *array);
extern "C" MObject *MArrayItem  (MArray *array, int index);

//------------------------------------------------------------------------------
//MImage:

typedef void (*MImageDispose)(int managedId);

class MImage : public MObject {};

extern "C" MImage *MImageCreate(int managedId, MImageDispose dispose);
extern "C" int MImageManagedId(MImage *image);

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
