#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "mexport.h"

typedef int MType;

const MType MType_Void    = 'v';    //void.
const MType MType_Bool    = 'b';    //bool.
const MType MType_Int     = 'i';    //int.
const MType MType_Float   = 'f';    //float.
const MType MType_C8Ptr   = '1';    //char *, uint8_t *.
const MType MType_C16Ptr  = '2';    //char16_t *.
const MType MType_MObject = 'O';    //MObject.
const MType MType_MBool   = 'B';    //MBool.
const MType MType_MInt    = 'I';    //MInt.
const MType MType_MFloat  = 'F';    //MFloat.
const MType MType_MString = 'S';    //MString.
const MType MType_MArray  = 'A';    //MArray.
const MType MType_MLambda = 'L';    //MLamdba.
const MType MType_MData   = 'D';    //MData.

//MObject:

struct MObject {

    MObject(MType type): RefCount(1), Type(type) {}
    virtual ~MObject() {}

    int   RefCount;
    MType Type;
};

extern "C" MType MGetType(MObject *object);
extern "C" void  MRetain (MObject *object);
extern "C" void  MRelease(MObject *object);

MEXPORT(MGetType)
MEXPORT(MRetain )
MEXPORT(MRelease)

//MBool:

struct MBool : MObject {
    
    MBool() : MObject(MType_MBool) {}

    bool Value = false;
};

extern "C" MBool *MBoolCreate(bool   value );
extern "C" int    MBoolValue (MBool *object);

//MInt:

struct MInt : MObject {
    
    MInt() : MObject(MType_MInt) {}

    int Value = 0;
};

extern "C" MInt *MIntCreate(int   value );
extern "C" int   MIntValue (MInt *object);

MEXPORT(MIntCreate)
MEXPORT(MIntValue )

//MFloat:

struct MFloat : MObject {

    MFloat() : MObject(MType_MFloat) {}

    float Value = 0;
};

extern "C" MFloat *MFloatCreate(float   value );
extern "C" float   MFloatValue (MFloat *object);

MEXPORT(MFloatCreate)
MEXPORT(MFloatValue )

//MString:

struct MString : MObject {
    
    MString() : MObject(MType_MString) {}

    std::string    U8string ;
    std::u16string U16string;
};

extern "C" MString *MStringCreateU16s(const char16_t *chars);
extern "C" MString *MStringCreateU8s (const char     *chars);

extern "C" const char16_t *MStringU16Bytes(MString *string);
extern "C" const char     *MStringU8Bytes (MString *string);

extern "C" int MStringU16Size(MString *string);
extern "C" int MStringU8Size (MString *string);

MEXPORT(MStringCreateU16s)
MEXPORT(MStringCreateU8s )
MEXPORT(MStringU16Bytes  )
MEXPORT(MStringU8Bytes   )
MEXPORT(MStringU16Size   )
MEXPORT(MStringU8Size    )

//MArray:

struct MArray : MObject {
    
    MArray() : MObject(MType_MArray) {}
    ~MArray();

    std::vector<MObject *> Items;
};

extern "C" MArray  *MArrayCreate();
extern "C" void     MArrayAppend(MArray *array, MObject *item);
extern "C" int      MArrayLength(MArray *array);
extern "C" MObject *MArrayItem  (MArray *array, int index);

MEXPORT(MArrayCreate)
MEXPORT(MArrayAppend)
MEXPORT(MArrayLength)
MEXPORT(MArrayItem  )

//MLambda:

typedef void (*MLambdaFunc)(MObject *load, MObject *param);

struct MLambda : MObject {
    
    MLambda() : MObject(MType_MLambda) {}
    ~MLambda();

    MLambdaFunc Func = NULL;
    MObject    *Load = NULL;
};

extern "C" MLambda *MLambdaCreate(MLambdaFunc func, MObject *load);

extern "C" void MLambdaCall(MLambda *lambda, MObject *param);

MEXPORT(MLambdaCreate)
MEXPORT(MLambdaCall  )

//MData:

struct MData : MObject {
    
    MData() : MObject(MType_MData) {}

    std::vector<uint8_t> Bytes;
};

extern "C" MData *MDataCreate(uint8_t *bytes, int size);

extern "C" const uint8_t *MDataBytes(MData *data);
extern "C" int MDataSize(MData *data);

MEXPORT(MDataCreate)
MEXPORT(MDataBytes )
MEXPORT(MDataSize  )

//shared ptrs:

template<class T>
std::shared_ptr<T> MMakeShared(T *ptr) {
    MRetain(ptr);
    return std::shared_ptr<T>(ptr, MRelease);
}

typedef std::shared_ptr<MObject> MObjectRef;
typedef std::shared_ptr<MBool  > MBoolRef  ;
typedef std::shared_ptr<MInt   > MIntRef   ;
typedef std::shared_ptr<MFloat > MFloatRef ;
typedef std::shared_ptr<MString> MStringRef;
typedef std::shared_ptr<MArray > MArrayRef ;
typedef std::shared_ptr<MLambda> MLambdaRef;
typedef std::shared_ptr<MData  > MDataRef  ;
