#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "mencode.h"

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

//MObject:

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

//MBool & MInt & MFloat:

template<typename T, int ID> class _MNumber : public MObject {

public:
    _MNumber(T value) : MObject(ID) {
        mValue = value;
    }

    T value() {
        return mValue;
    }

private:
    T mValue;
};

typedef _MNumber<bool , MType_Bool > MBool ;
typedef _MNumber<int  , MType_Int  > MInt  ;
typedef _MNumber<float, MType_Float> MFloat;

extern "C" inline MBool  *MBoolCreate (bool  v) { return new MBool (v); }
extern "C" inline MInt   *MIntCreate  (int   v) { return new MInt  (v); }
extern "C" inline MFloat *MFloatCreate(float v) { return new MFloat(v); }

extern "C" inline bool  MBoolValue (MBool  *a) { return a ? a->value() : 0; }
extern "C" inline int   MIntValue  (MInt   *a) { return a ? a->value() : 0; }
extern "C" inline float MFloatValue(MFloat *a) { return a ? a->value() : 0; }

//MString:

class MString : public MObject {

public:
    MString(const char *chars) : MObject(MType_MString) {
        if (chars) {
            mU8String  = chars;
            mU16String = MU16StringFromU8(chars);
        }
    }

    MString(const char16_t *chars) : MObject(MType_MString) {
        if (chars) {
            mU16String = chars;
            mU8String  = MU8StringFromU16(chars);
        }
    }

    const char     *u8Bytes () { return mU8String .c_str(); }
    const char16_t *u16Bytes() { return mU16String.c_str(); }

    int u8Size () { return (int)mU8String .size(); }
    int u16Size() { return (int)mU16String.size(); }
    
private:
    std::string    mU8String ;
    std::u16string mU16String;
};

extern "C" inline MString *MStringCreateU8 (const char     *s) { return new MString(s); }
extern "C" inline MString *MStringCreateU16(const char16_t *s) { return new MString(s); }

extern "C" inline const char     *MStringU8Bytes (MString *s) { return s ? s->u8Bytes () : nullptr; }
extern "C" inline const char16_t *MStringU16Bytes(MString *s) { return s ? s->u16Bytes() : nullptr; }

extern "C" inline int MStringU8Size (MString *s) { return s ? s->u8Size () : 0; }
extern "C" inline int MStringU16Size(MString *s) { return s ? s->u16Size() : 0; }

//MLambda:

typedef void (*MLambdaFunc)(MObject *load, MObject *param);

class MLambda : public MObject {
    
public:
    MLambda(MLambdaFunc func, MObject *load) : MObject(MType_MLambda) {
        MRetain(load);

        mFunc = func;
        mLoad = load;
    }

    ~MLambda() {
        MRelease(mLoad);
    }

    void call(MObject *param) {
        mFunc(mLoad, param);
    }

private:
    MLambdaFunc mFunc;
    MObject *mLoad;
};

extern "C" inline MLambda *MLambdaCreate(MLambdaFunc func, MObject *load) {
    return new MLambda(func, load);
}

extern "C" inline void MLambdaCall(MLambda *lambda, MObject *param) {
    if (lambda) {
        lambda->call(param);
    }
}

//MData:

class MData : public MObject {
    
public:
    MData(const uint8_t *bytes, int size) : MObject(MType_MData) {
        append(bytes, size);
    }

    void append(const uint8_t *bytes, int size) {
        if (bytes && size > 0) {
            mBytes.insert(mBytes.end(), bytes, bytes + size);
        }
    }

    const uint8_t *bytes() {
        return &mBytes[0];
    }

    int size() {
        return (int)mBytes.size();
    }

private:
    std::vector<uint8_t> mBytes;
};

extern "C" inline MData *MDataCreate(uint8_t *bytes, int size) {
    return new MData(bytes, size);
}

extern "C" inline void MDataAppend(MData *data, const uint8_t *bytes, int size) {
    if (data) {
        data->append(bytes, size);
    }
}

extern "C" inline const uint8_t *MDataBytes(MData *data) {
    return data ? data->bytes() : nullptr;
}

extern "C" inline int MDataSize(MData *data) {
    return data ? data->size() : 0;
}

//MArray:

class MArray : public MObject {
    
public:
    MArray();
    ~MArray();

    void append(MObject *item);
    int length();
    MObject *item(int index);

private:
    std::vector<MObject *> mItems;
};

extern "C" inline MArray *MArrayCreate() {
    return new MArray();
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

//MImage:

typedef void (*MImageDispose)(int id);

class MImage : public MObject {
    
public:
    MImage(int id, MImageDispose dispose) : MObject(MType_MImage) {
        mId = id;
        mDispose = dispose;
    }

    ~MImage() {
        mDispose(mId);
    }

    int id() {
        return mId;
    }

private:
    int mId = 0;
    MImageDispose mDispose = nullptr;
};

extern "C" inline MImage *MImageCreate(int id, MImageDispose dispose) {
    return new MImage(id, dispose);
}

extern "C" inline int MImageId(MImage *image) {
    return image ? image->id() : 0;
}
