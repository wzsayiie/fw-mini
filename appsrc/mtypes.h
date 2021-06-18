#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "mencode.h"
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
const MType MType_MLambda = 'L';    //MLamdba.
const MType MType_MData   = 'D';    //MData.
const MType MType_MArray  = 'A';    //MArray.

//MObject:

typedef std::shared_ptr<class MObject> MObjectRef;

class MObject {

public:
    void retain() {
        mRefCount += 1;
    }

    void release() {
        if (--mRefCount == 0) {
            delete this;
        }
    }

    MType type() {
        return mType;
    }

protected:
    MObject(MType type) {
        mRefCount = 1;
        mType = type;
    }

    virtual ~MObject() {
    }

private:
    int   mRefCount;
    MType mType;
};

extern "C" inline void MRetain(MObject *object) {
    if (object) {
        object->retain();
    }
}

extern "C" inline void MRelease(MObject *object) {
    if (object) {
        object->release();
    }
}

extern "C" inline MType MGetType(MObject *object) {
    return object ? object->type() : 0;
}

MEXPORT(MRetain )
MEXPORT(MRelease)
MEXPORT(MGetType)

//MBool & MInt & MFloat:

template<typename T, int ID>
class _MNumber : public MObject {

public:
    static _MNumber *create(T value) {
        return new _MNumber(value);
    }

    static std::shared_ptr<_MNumber> make(T value) {
        return std::shared_ptr<_MNumber>(create(value), MRelease);
    }

    T value() {
        return mValue;
    }

private:
    _MNumber(T value) : MObject(ID) {
        mValue = value;
    }

    T mValue;
};

typedef _MNumber<bool , MType_Bool > MBool ;
typedef _MNumber<int  , MType_Int  > MInt  ;
typedef _MNumber<float, MType_Float> MFloat;

typedef std::shared_ptr<MBool > MBoolRef ;
typedef std::shared_ptr<MInt  > MIntRef  ;
typedef std::shared_ptr<MFloat> MFloatRef;

extern "C" inline MBool  *MBoolCreate (bool  val) {return MBool ::create(val);}
extern "C" inline MInt   *MIntCreate  (int   val) {return MInt  ::create(val);}
extern "C" inline MFloat *MFloatCreate(float val) {return MFloat::create(val);}

extern "C" inline bool  MBoolValue (MBool  *obj) {return obj ? obj->value() : 0;}
extern "C" inline int   MIntValue  (MInt   *obj) {return obj ? obj->value() : 0;}
extern "C" inline float MFloatValue(MFloat *obj) {return obj ? obj->value() : 0;}

MEXPORT(MBoolCreate )
MEXPORT(MIntCreate  )
MEXPORT(MFloatCreate)
MEXPORT(MBoolValue  )
MEXPORT(MIntValue   )
MEXPORT(MFloatValue )

//MString:

typedef std::shared_ptr<class MString> MStringRef;

class MString : public MObject {

public:
    static MString *create(const char *chars) {
        return new MString(chars);
    }

    static MStringRef make(const char *chars) {
        return MStringRef(create(chars), MRelease);
    }

    static MString *create(const char16_t *chars) {
        return new MString(chars);
    }

    static MStringRef make(const char16_t *chars) {
        return MStringRef(create(chars), MRelease);
    }

    const char *u8Bytes() {
        return mU8String.c_str();
    }

    int u8Size() {
        return (int)mU8String.size();
    }

    const char16_t *u16Bytes() {
        return mU16String.c_str();
    }

    int u16Size() {
        return (int)mU16String.size();
    }
    
private:
    MString(const char *chars) : MObject(MType_MString) {
        if (chars) {
            mU8String = chars;
            mU16String = MU16StringFromU8(chars);
        }
    }

    MString(const char16_t *chars) : MObject(MType_MString) {
        if (chars) {
            mU16String = chars;
            mU8String  = MU8StringFromU16(chars);
        }
    }

    std::string    mU8String ;
    std::u16string mU16String;
};

extern "C" inline MString *MStringCreateU8(const char *chars) {
    return MString::create(chars);
}

extern "C" inline MString *MStringCreateU16(const char16_t *chars) {
    return MString::create(chars);
}

extern "C" inline const char *MStringU8Bytes(MString *string) {
    return string ? string->u8Bytes() : nullptr;
}

extern "C" inline int MStringU8Size(MString *string) {
    return string ? string->u8Size() : 0;
}

extern "C" inline const char16_t *MStringU16Bytes(MString *string) {
    return string ? string->u16Bytes() : nullptr;
}

extern "C" inline int MStringU16Size(MString *string) {
    return string ? string->u16Size() : 0;
}

MEXPORT(MStringCreateU8 )
MEXPORT(MStringCreateU16)
MEXPORT(MStringU8Bytes  )
MEXPORT(MStringU8Size   )
MEXPORT(MStringU16Bytes )
MEXPORT(MStringU16Size  )

//MLambda:

typedef void (*MLambdaFunc)(MObject *load, MObject *param);

typedef std::shared_ptr<class MLambda> MLambdaRef;

class MLambda : public MObject {
    
public:
    static MLambda *create(MLambdaFunc func, MObject *load) {
        return func ? new MLambda(func, load) : nullptr;
    }

    static MLambdaRef make(MLambdaFunc func, MObject *load) {
        return MLambdaRef(create(func, load), MRelease);
    }

    void call(MObject *param) {
        mFunc(mLoad, param);
    }

private:
    MLambda(MLambdaFunc func, MObject *load) : MObject(MType_MLambda) {
        MRetain(load);

        mFunc = func;
        mLoad = load;
    }

    ~MLambda() {
        MRelease(mLoad);
    }

    MLambdaFunc mFunc;
    MObject *mLoad;
};

extern "C" inline MLambda *MLambdaCreate(MLambdaFunc func, MObject *load) {
    return MLambda::create(func, load);
}

extern "C" inline void MLambdaCall(MLambda *lambda, MObject *param) {
    if (lambda) {
        lambda->call(param);
    }
}

MEXPORT(MLambdaCreate)
MEXPORT(MLambdaCall  )

//MData:

typedef std::shared_ptr<class MData> MDataRef;

class MData : public MObject {
    
public:
    static MData *create(const uint8_t *bytes, int size) {
        return new MData(bytes, size);
    }

    static MDataRef make(const uint8_t *bytes, int size) {
        return MDataRef(create(bytes, size), MRelease);
    }

    const uint8_t *bytes() {
        return &mBytes[0];
    }

    int size() {
        return (int)mBytes.size();
    }

private:
    MData(const uint8_t *bytes, int size) : MObject(MType_MData) {
        mBytes.insert(mBytes.end(), bytes, bytes + size);
    }

    std::vector<uint8_t> mBytes;
};

extern "C" inline MData *MDataCreate(uint8_t *bytes, int size) {
    return MData::create(bytes, size);
}

extern "C" inline const uint8_t *MDataBytes(MData *data) {
    return data ? data->bytes() : nullptr;
}

extern "C" inline int MDataSize(MData *data) {
    return data ? data->size() : 0;
}

MEXPORT(MDataCreate)
MEXPORT(MDataBytes )
MEXPORT(MDataSize  )

//MArray:

typedef std::shared_ptr<class MArray> MArrayRef;

class MArray : public MObject {
    
public:
    static MArray *create();
    static MArrayRef make();

    void     append(MObject *item);
    int      length();
    MObject *item  (int index);

    ~MArray();

private:
    MArray() : MObject(MType_MArray) {
    }

    std::vector<MObject *> mItems;
};

extern "C" inline MArray *MArrayCreate() {
    return MArray::create();
}

extern "C" inline void MArrayAppend(MArray *array, MObject *item) {
    if (array) {
        array->append(item);
    }
}

extern "C" inline int MArrayLength(MArray *array) {
    return array ? array->length() : 0;
}

extern "C" inline MObject *MArrayItem(MArray *array, int index) {
    return array ? array->item(index) : 0;
}

MEXPORT(MArrayCreate)
MEXPORT(MArrayAppend)
MEXPORT(MArrayLength)
MEXPORT(MArrayItem  )

//MObject* to MObjectRef:

MObjectRef MMakeShared(MObject *value);
MBoolRef   MMakeShared(MBool   *value);
MIntRef    MMakeShared(MInt    *value);
MFloatRef  MMakeShared(MFloat  *value);
MStringRef MMakeShared(MString *value);
MLambdaRef MMakeShared(MLambda *value);
MDataRef   MMakeShared(MData   *value);
MArrayRef  MMakeShared(MArray  *value);
