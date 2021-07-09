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
    void _retain() {
        mRefCount += 1;
    }

    void _release() {
        if (--mRefCount == 0) {
            delete this;
        }
    }

    MType _type() {
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
        object->_retain();
    }
}

extern "C" inline void MRelease(MObject *object) {
    if (object) {
        object->_release();
    }
}

extern "C" inline MType MGetType(MObject *object) {
    return object ? object->_type() : 0;
}

//MBool & MInt & MFloat:

template<typename T, int ID> class _MNumber : public MObject {

public:
    _MNumber(T value) : MObject(ID) {
        mValue = value;
    }

    T _value() {
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

extern "C" inline bool  MBoolValue (MBool  *a) { return a ? a->_value() : 0; }
extern "C" inline int   MIntValue  (MInt   *a) { return a ? a->_value() : 0; }
extern "C" inline float MFloatValue(MFloat *a) { return a ? a->_value() : 0; }

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

    const char     *_u8Bytes () { return mU8String .c_str(); }
    const char16_t *_u16Bytes() { return mU16String.c_str(); }

    int _u8Size () { return (int)mU8String .size(); }
    int _u16Size() { return (int)mU16String.size(); }
    
private:
    std::string    mU8String ;
    std::u16string mU16String;
};

extern "C" inline MString *MStringCreateU8 (const char     *s) { return new MString(s); }
extern "C" inline MString *MStringCreateU16(const char16_t *s) { return new MString(s); }

extern "C" inline const char     *MStringU8Bytes (MString *s) { return s ? s->_u8Bytes () : nullptr; }
extern "C" inline const char16_t *MStringU16Bytes(MString *s) { return s ? s->_u16Bytes() : nullptr; }

extern "C" inline int MStringU8Size (MString *s) { return s ? s->_u8Size () : 0; }
extern "C" inline int MStringU16Size(MString *s) { return s ? s->_u16Size() : 0; }

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

    void _call(MObject *param) {
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
        lambda->_call(param);
    }
}

//MData:

class MData : public MObject {
    
public:
    MData(const uint8_t *bytes, int size) : MObject(MType_MData) {
        _append(bytes, size);
    }

    void _append(const uint8_t *bytes, int size) {
        if (bytes && size > 0) {
            mBytes.insert(mBytes.end(), bytes, bytes + size);
        }
    }

    const uint8_t *_bytes() {
        return &mBytes[0];
    }

    int _size() {
        return (int)mBytes.size();
    }

private:
    std::vector<uint8_t> mBytes;
};

extern "C" inline MData *MDataCreate(const uint8_t *bytes, int size) {
    return new MData(bytes, size);
}

extern "C" inline void MDataAppend(MData *data, const uint8_t *bytes, int size) {
    if (data) {
        data->_append(bytes, size);
    }
}

extern "C" inline const uint8_t *MDataBytes(MData *data) {
    return data ? data->_bytes() : nullptr;
}

extern "C" inline int MDataSize(MData *data) {
    return data ? data->_size() : 0;
}

//MArray:

class MArray : public MObject {
    
public:
    MArray();
    ~MArray();

    void _append(MObject *item);
    int _length();
    MObject *_item(int index);

private:
    std::vector<MObject *> mItems;
};

extern "C" inline MArray *MArrayCreate() {
    return new MArray();
}

extern "C" inline void MArrayAppend(MArray *array, MObject *item) {
    if (array) {
        array->_append(item);
    }
}

extern "C" inline int MArrayLength(MArray *array) {
    return array ? array->_length() : 0;
}

extern "C" inline MObject *MArrayItem(MArray *array, int index) {
    return array ? array->_item(index) : 0;
}

//MImage:

typedef void (*MImageDispose)(int id);

class MImage : public MObject {
    
public:
    MImage(int managedId, MImageDispose dispose) : MObject(MType_MImage) {
        mManagedId = managedId;
        mDispose = dispose;
    }

    ~MImage() {
        mDispose(mManagedId);
    }

    int _managedId() {
        return mManagedId;
    }

private:
    int mManagedId = 0;
    MImageDispose mDispose = nullptr;
};

extern "C" inline MImage *MImageCreate(int managedId, MImageDispose dispose) {
    return new MImage(managedId, dispose);
}

extern "C" inline int MImageManagedId(MImage *image) {
    return image ? image->_managedId() : 0;
}
