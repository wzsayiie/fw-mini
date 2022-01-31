#include "mbasics.h"
#include "mencode.h"

//------------------------------------------------------------------------------
//MObject:

void MObject::_retain() {
    mRefCount += 1;

    if (mRefObserver) {
        mRefObserver(this, mRefCount);
    }
}

void MObject::_release() {
    mRefCount -= 1;

    if (mRefObserver) {
        mRefObserver(this, mRefCount);
    }

    if (mRefCount == 0) {
        delete this;
    }
}

void MObject::_setRefObserver(_MRefObserver observer) {
    mRefObserver = observer;
}

int MObject::_refCount() {
    return mRefCount;
}

MObject *MRetain(MObject *object) {
    if (object) {
        object->_retain();
    }
    return object;
}

void MRelease(MObject *object) {
    if (object) {
        object->_release();
    }
}

MTypeId MGetTypeId(MObject *object) {
    if (object) {
        return object->_typeId();
    }
    return 0;
}

//------------------------------------------------------------------------------
//MBool & MInt & MFloat & MPtr:

template<typename T, typename SUPER> struct _Number : SUPER {

    T value;
    
    _Number(T value) {
        this->value = value;
    }
};

typedef _Number<bool     , MBool > MBoolImpl ;
typedef _Number<int      , MInt  > MIntImpl  ;
typedef _Number<float    , MFloat> MFloatImpl;
typedef _Number<uint8_t *, MPtr  > MPtrImpl  ;

MBool  *MBoolCreate (bool     value) { return new MBoolImpl (value); }
MInt   *MIntCreate  (int      value) { return new MIntImpl  (value); }
MFloat *MFloatCreate(float    value) { return new MFloatImpl(value); }
MPtr   *MPtrCreate  (uint8_t *value) { return new MPtrImpl  (value); }

bool     MBoolValue (MBool  *obj) { return obj ? ((MBoolImpl  *)obj)->value : 0; }
int      MIntValue  (MInt   *obj) { return obj ? ((MIntImpl   *)obj)->value : 0; }
float    MFloatValue(MFloat *obj) { return obj ? ((MFloatImpl *)obj)->value : 0; }
uint8_t *MPtrValue  (MPtr   *obj) { return obj ? ((MPtrImpl   *)obj)->value : 0; }

//------------------------------------------------------------------------------
//MString:

struct MStringImpl : MString {

    std::string    u8string ;
    std::u16string u16string;
    
    MStringImpl(const char *chars, int length) {
        if (length > 0) {
            u8string.append(chars, chars + length);
        } else {
            u8string.append(chars);
        }
    }

    MStringImpl(const char16_t *chars, int length) {
        if (length > 0) {
            u16string.append(chars, chars + length);
        } else {
            u16string.append(chars);
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
};

MString *MStringCreateU8 (const char     *chs) { return chs && *chs ? new MStringImpl(chs, 0) : nullptr; }
MString *MStringCreateU16(const char16_t *chs) { return chs && *chs ? new MStringImpl(chs, 0) : nullptr; }

MString *MStringCopyU8 (const char     *bgn, int len) { return bgn && len > 0 ? new MStringImpl(bgn, len) : nullptr; }
MString *MStringCopyU16(const char16_t *bgn, int len) { return bgn && len > 0 ? new MStringImpl(bgn, len) : nullptr; }

const char     *MStringU8Chars (MString *str) { return str ? ((MStringImpl *)str)->u8s ().c_str() : nullptr; }
const char16_t *MStringU16Chars(MString *str) { return str ? ((MStringImpl *)str)->u16s().c_str() : nullptr; }

int MStringU8Size (MString *str) { return str ? (int)((MStringImpl *)str)->u8s ().size() : 0; }
int MStringU16Size(MString *str) { return str ? (int)((MStringImpl *)str)->u16s().size() : 0; }

//------------------------------------------------------------------------------
//MLambda:

struct MLambdaImpl : MLambda {
    
    MLambdaFunc func;
    MObject    *load;
    
    MLambdaImpl(MLambdaFunc func, MObject *load) {
        MRetain(load);
        this->func = func;
        this->load = load;
    }

    ~MLambdaImpl() {
        MRelease(load);
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
        object->func(object->load);
    }
}

//------------------------------------------------------------------------------
//MData:

struct MDataImpl : MData {
    std::vector<uint8_t> store;
};

inline std::vector<uint8_t> *GetDataStore(MData *data) {
    return data ? &((MDataImpl *)data)->store : nullptr;
}

MData *MDataCopy(const uint8_t *bytes, int size) {
    auto data = new MDataImpl();
    if (bytes && size > 0) {
        std::vector<uint8_t> *store = GetDataStore(data);
        store->insert(store->end(), bytes, bytes + size);
    }
    return data;
}

MData *MDataCreate(int size) {
    auto data = new MDataImpl();
    if (size > 0) {
        data->store.resize(size);
    }
    return data;
}

void MDataAppend(MData *data, const uint8_t *bytes, int size) {
    std::vector<uint8_t> *store = GetDataStore(data);
    if (store && bytes && size > 0) {
        store->insert(store->end(), bytes, bytes + size);
    }
}

uint8_t *MDataBytes(MData *data) {
    std::vector<uint8_t> *store = GetDataStore(data);
    if (store) {
        return store->data();
    }
    return 0;
}

uint8_t *MDataEnd(MData *data) {
    std::vector<uint8_t> *store = GetDataStore(data);
    if (store) {
        return store->data() + store->size();
    }
    return 0;
}

int MDataSize(MData *data) {
    std::vector<uint8_t> *store = GetDataStore(data);
    if (store) {
        return (int)store->size();
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
//lambda cast:

class LambdaCastWrapper : public MUnknown {

public:
    LambdaCastWrapper(std::function<void ()> func) {
        mFunc = func;
    }

    static void call(MObject *load) {
        auto wrapper = (LambdaCastWrapper *)load;
        wrapper->mFunc();
    }

private:
    std::function<void ()> mFunc;
};

MLambdaRef _MLambdaCastHelper::operator<<(std::function<void ()> func) {
    return m_auto_release MLambdaCreate(LambdaCastWrapper::call, new LambdaCastWrapper(func));
}
