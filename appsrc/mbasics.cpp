#include "mbasics.h"
#include "mencode.h"

//------------------------------------------------------------------------------
//MObject:

MObject *MRetain(MObject *object) {
    return (MObject *)retain(object);
}

void MRelease(MObject *object) {
    release(object);
}

MTypeId MGetTypeId(MObject *object) {
    if (object) {
        return object->typeId();
    }
    return 0;
}

//------------------------------------------------------------------------------
//number types:

bool MIsNumberObject(MTypeId type) {
    if (type == MTypeIdOf<MBool   *>::Value) { return true; }
    if (type == MTypeIdOf<MInt    *>::Value) { return true; }
    if (type == MTypeIdOf<MInt64  *>::Value) { return true; }
    if (type == MTypeIdOf<MFloat  *>::Value) { return true; }
    if (type == MTypeIdOf<MDouble *>::Value) { return true; }
    
    return false;
}

template<typename T, typename SUPER> struct Number : SUPER {
    T value;
    
    Number(T value) {
        this->value = value;
    }
};

typedef Number<bool   , MBool  > MBoolImpl  ;
typedef Number<int    , MInt   > MIntImpl   ;
typedef Number<int64_t, MInt64 > MInt64Impl ;
typedef Number<float  , MFloat > MFloatImpl ;
typedef Number<double , MDouble> MDoubleImpl;

MBool   *MBoolCreate  (bool    value) { return new MBoolImpl  (value); }
MInt    *MIntCreate   (int     value) { return new MIntImpl   (value); }
MInt64  *MInt64Create (int64_t value) { return new MInt64Impl (value); }
MFloat  *MFloatCreate (float   value) { return new MFloatImpl (value); }
MDouble *MDoubleCreate(double  value) { return new MDoubleImpl(value); }

static int64_t GetInt64(MObject *object, bool *got) {
    *got = true;
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MBool   *>::Value: return (int64_t)((MBoolImpl   *)object)->value;
        case MTypeIdOf<MInt    *>::Value: return (int64_t)((MIntImpl    *)object)->value;
        case MTypeIdOf<MInt64  *>::Value: return /* ... */((MInt64Impl  *)object)->value;
        case MTypeIdOf<MFloat  *>::Value: return (int64_t)((MFloatImpl  *)object)->value;
        case MTypeIdOf<MDouble *>::Value: return (int64_t)((MDoubleImpl *)object)->value;
    }
    
    *got = false;
    return 0;
}

static double GetDouble(MObject *object, bool *got) {
    *got = true;
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MBool   *>::Value: return (double)((MBoolImpl   *)object)->value;
        case MTypeIdOf<MInt    *>::Value: return (double)((MIntImpl    *)object)->value;
        case MTypeIdOf<MInt64  *>::Value: return (double)((MInt64Impl  *)object)->value;
        case MTypeIdOf<MFloat  *>::Value: return (double)((MFloatImpl  *)object)->value;
        case MTypeIdOf<MDouble *>::Value: return /* .. */((MDoubleImpl *)object)->value;
    }
    
    *got = false;
    return 0;
}

static bool GetBool(MObject *object, bool *got) {
    int64_t value = GetInt64(object, got);
    return *got ? (bool)value : object != nullptr;
}

static int   GetInt  (MObject *object, bool *got) { return (int  )GetInt64 (object, got); }
static float GetFloat(MObject *object, bool *got) { return (float)GetDouble(object, got); }

MBool   *MBoolCopy  (MObject *a) { bool g; bool    v = GetBool  (a, &g); return g ? new MBoolImpl  (v) : nullptr; }
MInt    *MIntCopy   (MObject *a) { bool g; int     v = GetInt   (a, &g); return g ? new MIntImpl   (v) : nullptr; }
MInt64  *MInt64Copy (MObject *a) { bool g; int64_t v = GetInt64 (a, &g); return g ? new MInt64Impl (v) : nullptr; }
MFloat  *MFloatCopy (MObject *a) { bool g; float   v = GetFloat (a, &g); return g ? new MFloatImpl (v) : nullptr; }
MDouble *MDoubleCopy(MObject *a) { bool g; double  v = GetDouble(a, &g); return g ? new MDoubleImpl(v) : nullptr; }

bool    MBoolValue  (MObject *object) { bool got; return GetBool  (object, &got); }
int     MIntValue   (MObject *object) { bool got; return GetInt   (object, &got); }
int64_t MInt64Value (MObject *object) { bool got; return GetInt64 (object, &got); }
float   MFloatValue (MObject *object) { bool got; return GetFloat (object, &got); }
double  MDoubleValue(MObject *object) { bool got; return GetDouble(object, &got); }

//------------------------------------------------------------------------------
//MPtr:

struct MPtrImpl : MPtr {
    uint8_t *value;
    
    MPtrImpl(uint8_t *value) {
        this->value = value;
    }
};

MPtr *MPtrCreate(uint8_t *value) {
    return new MPtrImpl(value);
}

uint8_t *MPtrValue(MPtr *object) {
    if (object) {
        return ((MPtrImpl *)object)->value;
    }
    return nullptr;
}

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

const char     *MStringU8Chars (MString *str) { return str ? ((MStringImpl *)str)->u8s ().c_str() :  ""; }
const char16_t *MStringU16Chars(MString *str) { return str ? ((MStringImpl *)str)->u16s().c_str() : u""; }

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
    return nullptr;
}

uint8_t *MDataEnd(MData *data) {
    std::vector<uint8_t> *store = GetDataStore(data);
    if (store) {
        return store->data() + store->size();
    }
    return nullptr;
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
//type cast:

class LambdaCastWrapper : public MUnknown {

public:
    LambdaCastWrapper(const std::function<void ()> &func) {
        mFunc = func;
    }

    static void call(MObject *load) {
        auto wrapper = (LambdaCastWrapper *)load;
        wrapper->mFunc();
    }

private:
    std::function<void ()> mFunc;
};

MLambdaRef _MLambdaCastHelper::operator<<(const std::function<void ()> &func) {
    return m_auto_release MLambdaCreate(LambdaCastWrapper::call, new LambdaCastWrapper(func));
}
