#include "mtypes.h"
#include <vector>
#include "mencode.h"

//------------------------------------------------------------------------------
//MObject:

void MObject::_retain() {
    mRefCount += 1;
}

void MObject::_release() {
    if (--mRefCount == 0) {
        delete this;
    }
}

void MRetain(MObject *object) {
    if (object) {
        object->_retain();
    }
}

void MRelease(MObject *object) {
    if (object) {
        object->_release();
    }
}

MType MGetType(MObject *object) {
    if (object) {
        return object->_type();
    }
    return 0;
}

//------------------------------------------------------------------------------
//MBool & MInt & MFloat:

template<typename T, int ID, typename BASE> struct _MNumber : BASE {

    T value;
    
    _MNumber(T value) {
        this->value = value;
    }

    MType _type() override {
        return ID;
    }
};

typedef _MNumber<bool , MType_Bool , MBool > MBoolImpl ;
typedef _MNumber<int  , MType_Int  , MInt  > MIntImpl  ;
typedef _MNumber<float, MType_Float, MFloat> MFloatImpl;

MBool  *MBoolCreate (bool  value) { return new MBoolImpl (value); }
MInt   *MIntCreate  (int   value) { return new MIntImpl  (value); }
MFloat *MFloatCreate(float value) { return new MFloatImpl(value); }

bool  MBoolValue (MBool  *obj) { return obj ? ((MBoolImpl  *)obj)->value : 0; }
int   MIntValue  (MInt   *obj) { return obj ? ((MIntImpl   *)obj)->value : 0; }
float MFloatValue(MFloat *obj) { return obj ? ((MFloatImpl *)obj)->value : 0; }

//------------------------------------------------------------------------------
//MString:

struct MStringImpl : MString {

    std::string    u8string ;
    std::u16string u16string;
    
    MStringImpl(const char *chars) {
        if (chars) {
            u8string = chars;
        }
    }

    MStringImpl(const char16_t *chars) {
        if (chars) {
            u16string = chars;
        }
    }
    
    const std::string &u8s() {
        if (u8string.empty() && !u16string.empty()) {
            u8string = MU8StringFromU16(u16string.c_str());
        }
        return u8string;
    }

    const std::u16string &u16s() {
        if (u16string.empty() && !u8string.empty()) {
            u16string = MU16StringFromU8(u8string.c_str());
        }
        return u16string;
    }
    
    MType _type() override {
        return MType_MString;
    }
};

MString *MStringCreateU8 (const char     *chs) { return chs ? new MStringImpl(chs) : nullptr; }
MString *MStringCreateU16(const char16_t *chs) { return chs ? new MStringImpl(chs) : nullptr; }

const char     *MStringU8Chars (MString *str) { return str ? ((MStringImpl *)str)->u8s ().c_str() : nullptr; }
const char16_t *MStringU16Chars(MString *str) { return str ? ((MStringImpl *)str)->u16s().c_str() : nullptr; }

int MStringU8Size (MString *str) { return str ? (int)((MStringImpl *)str)->u8s ().size() : 0; }
int MStringU16Size(MString *str) { return str ? (int)((MStringImpl *)str)->u16s().size() : 0; }

//------------------------------------------------------------------------------
//MLambda:

struct MLambdaImpl : MLambda {
    
    MLambdaFunc mFunc;
    MObject    *mLoad;
    
    MLambdaImpl(MLambdaFunc func, MObject *load) {
        MRetain(load);
        mFunc = func;
        mLoad = load;
    }

    ~MLambdaImpl() {
        MRelease(mLoad);
    }
    
    MType _type() override {
        return MType_MLambda;
    }
};

MLambda *MLambdaCreate(MLambdaFunc func, MObject *load) {
    if (func) {
        return new MLambdaImpl(func, load);
    }
    return nullptr;
}

void MLambdaCall(MLambda *lambda) {
    if (lambda) {
        auto object = (MLambdaImpl *)lambda;
        object->mFunc(object->mLoad);
    }
}

//------------------------------------------------------------------------------
//MData:

struct MDataImpl : MData {
    
    std::vector<uint8_t> bytes;
    
    MType _type() override {
        return MType_MData;
    }
};

MData *MDataCreate(const uint8_t *bytes, int size) {
    auto data = new MDataImpl();
    if (bytes && size > 0) {
        data->bytes.insert(data->bytes.end(), bytes, bytes + size);
    }
    return data;
}

void MDataAppend(MData *data, const uint8_t *bytes, int size) {
    if (data && bytes && size > 0) {
        auto object = (MDataImpl *)data;
        object->bytes.insert(object->bytes.end(), bytes, bytes + size);
    }
}

int MDataSize(MData *data) {
    if (data) {
        auto object = (MDataImpl *)data;
        return (int)object->bytes.size();
    }
    return 0;
}

const uint8_t *MDataBytes(MData *data) {
    if (data) {
        auto object = (MDataImpl *)data;
        return &object->bytes[0];
    }
    return 0;
}

//------------------------------------------------------------------------------
//MArray:

struct MArrayImpl : MArray {
    
    std::vector<MObject *> items;
    
    ~MArrayImpl() {
        for (MObject *it : items) {
            MRelease(it);
        }
    }

    MType _type() override {
        return MType_MArray;
    }
};

MArray *MArrayCreate() {
    return new MArrayImpl();
}

void MArrayAppend(MArray *array, MObject *item) {
    if (!array) {
        return;
    }
    
    MRetain(item);
    
    auto object = (MArrayImpl *)array;
    object->items.push_back(item);
}

int MArrayLength(MArray *array) {
    if (array) {
        auto object = (MArrayImpl *)array;
        return (int)object->items.size();
    }
    return 0;
}

MObject *MArrayItem(MArray *array, int index) {
    auto object = (MArrayImpl *)array;
    if (!object) {
        return nullptr;
    }
    
    if (index < 0 || (int)object->items.size() <= index) {
        return nullptr;
    }
    
    return object->items[index];
}

//------------------------------------------------------------------------------
//MImage:

struct MImageImpl : MImage {
    
    int managedId = 0;
    MImageDispose dispose = nullptr;

    MImageImpl(int managedId, MImageDispose dispose) {
        this->managedId = managedId;
        this->dispose = dispose;
    }
    
    ~MImageImpl() {
        dispose(managedId);
    }
    
    MType _type() override {
        return MType_MImage;
    }
};

MImage *MImageCreate(int managedId, MImageDispose dispose) {
    if (managedId != 0) {
        return new MImageImpl(managedId, dispose);
    }
    return nullptr;
}

int MImageManagedId(MImage *image) {
    if (image) {
        auto object = (MImageImpl *)image;
        return object->managedId;
    }
    return 0;
}
