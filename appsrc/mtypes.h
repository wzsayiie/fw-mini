#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "mencode.h"

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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
//MString:

class MString : public MObject {

public:
    MString(const char *chars) : MObject(MType_MString) {
        if (chars) {
            mU8String = chars;
        }
    }

    MString(const char16_t *chars) : MObject(MType_MString) {
        if (chars) {
            mU16String = chars;
        }
    }

    const char     *_u8chars () { return u8string ().c_str(); }
    const char16_t *_u16chars() { return u16string().c_str(); }

    int _u8size () { return (int)u8string ().size(); }
    int _u16size() { return (int)u16string().size(); }
    
private:
    const std::string &u8string() {
        if (mU8String.empty() && !mU16String.empty()) {
            mU8String = MU8StringFromU16(mU16String.c_str());
        }
        return mU8String;
    }

    const std::u16string &u16string() {
        if (mU16String.empty() && !mU8String.empty()) {
            mU16String = MU16StringFromU8(mU8String.c_str());
        }
        return mU16String;
    }

    std::string    mU8String ;
    std::u16string mU16String;
};

extern "C" inline MString *MStringCreateU8 (const char     *s) { return new MString(s); }
extern "C" inline MString *MStringCreateU16(const char16_t *s) { return new MString(s); }

extern "C" inline const char     *MStringU8Chars (MString *s) { return s ? s->_u8chars () : nullptr; }
extern "C" inline const char16_t *MStringU16Chars(MString *s) { return s ? s->_u16chars() : nullptr; }

extern "C" inline int MStringU8Size (MString *s) { return s ? s->_u8size () : 0; }
extern "C" inline int MStringU16Size(MString *s) { return s ? s->_u16size() : 0; }

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
//MImage:

typedef void (*MImageDispose)(int managedId);

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
