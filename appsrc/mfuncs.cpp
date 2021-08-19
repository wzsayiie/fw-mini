#include "mfuncs.h"

typedef std::map<std::string, _MFuncMeta> MetaMap;

static MetaMap *GetMetaMap() {
    static MetaMap *map = nullptr;
    if (!map) {
        map = new MetaMap;
    }
    return map;
}

static MetaMap::iterator *GetIterator() {
    static MetaMap::iterator *iterator = nullptr;
    if (!iterator) {
        iterator = new MetaMap::iterator;
    }
    return iterator;
}

#define sMetaMap  (*GetMetaMap ())
#define sIterator (*GetIterator())

void _MFuncSetMeta(const char *name, const _MFuncMeta& meta) {
    if (name) {
        sMetaMap[name] = meta;
    }
}

void MFuncSelectFirst() {
    sIterator = sMetaMap.begin();
}

bool MFuncSelectedValid() {
    return sIterator != sMetaMap.end();
}

void MFuncSelectNext() {
    ++sIterator;
}

bool MFuncSelect(const char *name) {
    if (name) {
        sIterator = sMetaMap.find(name);
    } else {
        sIterator = sMetaMap.end();
    }

    return sIterator != sMetaMap.end();
}

const char *MFuncSelectedName() {
    return sIterator->first.c_str();
}

MType MFuncSelectedRetType  () { return sIterator->second.retType  ; }
bool  MFuncSelectedRetRetain() { return sIterator->second.retRetain; }
int   MFuncSelectedArgCount () { return sIterator->second.argCount ; }

MType MFuncSelectedArgType(int index) {
    return sIterator->second.argTypes[index];
}

//NOTE:
//template instantiation parameters only use 3 types: void, float, and intptr_t,
//to prevent code bloat.
//

static intptr_t AsMObject(MObject *object) {
    return (intptr_t)object;
}

static intptr_t AsBool(MObject *object) {
    switch (MGetType(object)) {
        case MType_MBool : return (intptr_t)MBoolValue ((MBool  *)object);
        case MType_MInt  : return (intptr_t)MIntValue  ((MInt   *)object);
        case MType_MFloat: return (intptr_t)MFloatValue((MFloat *)object);

        default: return object != nullptr;
    }
}

static intptr_t AsInt(MObject *object) {
    switch (MGetType(object)) {
        case MType_MBool : return (intptr_t)MBoolValue ((MBool  *)object);
        case MType_MInt  : return (intptr_t)MIntValue  ((MInt   *)object);
        case MType_MFloat: return (intptr_t)MFloatValue((MFloat *)object);

        default: return 0;
    }
}

static float AsFloat(MObject *object) {
    switch (MGetType(object)) {
        case MType_MBool : return (float)MBoolValue ((MBool  *)object);
        case MType_MInt  : return (float)MIntValue  ((MInt   *)object);
        case MType_MFloat: return (float)MFloatValue((MFloat *)object);

        default: return 0;
    }
}

static intptr_t AsU8Chars(MObject *object) {
    if (MGetType(object) == MType_MString) {
        return (intptr_t)MStringU8Chars((MString *)object);
    }
    return 0;
}

static intptr_t AsU16Chars(MObject *object) {
    if (MGetType(object) == MType_MString) {
        return (intptr_t)MStringU16Chars((MString *)object);
    }
    return 0;
}

static intptr_t AsMBool  (MObject *a) { return MGetType(a) == MType_MBool   ? (intptr_t)a : 0; }
static intptr_t AsMInt   (MObject *a) { return MGetType(a) == MType_MInt    ? (intptr_t)a : 0; }
static intptr_t AsMFloat (MObject *a) { return MGetType(a) == MType_MFloat  ? (intptr_t)a : 0; }
static intptr_t AsMString(MObject *a) { return MGetType(a) == MType_MString ? (intptr_t)a : 0; }
static intptr_t AsMLambda(MObject *a) { return MGetType(a) == MType_MLambda ? (intptr_t)a : 0; }
static intptr_t AsMData  (MObject *a) { return MGetType(a) == MType_MData   ? (intptr_t)a : 0; }
static intptr_t AsMArray (MObject *a) { return MGetType(a) == MType_MArray  ? (intptr_t)a : 0; }
static intptr_t AsMImage (MObject *a) { return MGetType(a) == MType_MImage  ? (intptr_t)a : 0; }
static intptr_t AsMNative(MObject *a) { return MGetType(a) == MType_MNative ? (intptr_t)a : 0; }

template<typename R, int N> struct Caller {

    template<typename... U> static R Run(const _MFuncMeta &meta, MArray *args, U... unfold) {

        if (N == meta.argCount) {
            return ((R (*)(U...))meta.address)(unfold...);
        }

        MObject *item = MArrayItem(args, N);
        MType type = meta.argTypes[N];

        switch (type) {
            case MType_Bool   : return Caller<R, N + 1>::Run(meta, args, unfold..., AsBool    (item));
            case MType_Int    : return Caller<R, N + 1>::Run(meta, args, unfold..., AsInt     (item));
            case MType_Float  : return Caller<R, N + 1>::Run(meta, args, unfold..., AsFloat   (item));
            case MType_C8Ptr  : return Caller<R, N + 1>::Run(meta, args, unfold..., AsU8Chars (item));
            case MType_C16Ptr : return Caller<R, N + 1>::Run(meta, args, unfold..., AsU16Chars(item));
            case MType_MObject: return Caller<R, N + 1>::Run(meta, args, unfold..., AsMObject (item));
            case MType_MBool  : return Caller<R, N + 1>::Run(meta, args, unfold..., AsMBool   (item));
            case MType_MInt   : return Caller<R, N + 1>::Run(meta, args, unfold..., AsMInt    (item));
            case MType_MFloat : return Caller<R, N + 1>::Run(meta, args, unfold..., AsMFloat  (item));
            case MType_MString: return Caller<R, N + 1>::Run(meta, args, unfold..., AsMString (item));
            case MType_MLambda: return Caller<R, N + 1>::Run(meta, args, unfold..., AsMLambda (item));
            case MType_MData  : return Caller<R, N + 1>::Run(meta, args, unfold..., AsMData   (item));
            case MType_MArray : return Caller<R, N + 1>::Run(meta, args, unfold..., AsMArray  (item));
            case MType_MImage : return Caller<R, N + 1>::Run(meta, args, unfold..., AsMImage  (item));
            case MType_MNative: return Caller<R, N + 1>::Run(meta, args, unfold..., AsMNative (item));

            default: return (R)0;
        }
    }
};

template<typename R> struct Caller<R, MFuncMaxArgCount> {

    template<typename... U> static R Run(const _MFuncMeta &meta, MArray *args, U... unfold) {
        return ((R (*)(U...))meta.address)(unfold...);
    }
};

MObject *MFuncCallCopyRet(const char *name, MArray *args) {
    if (!name) {
        return nullptr;
    }

    auto iterator = sMetaMap.find(name);
    if (iterator == sMetaMap.end()) {
        return nullptr;
    }

    _MFuncMeta meta = iterator->second;
    bool rr = meta.retRetain;
    switch (meta.retType) {
        case MType_Void   : { /*  void return  */  Caller<void    , 0>::Run(meta, args); return nullptr            ; }
        case MType_Bool   : { auto v = (bool      )Caller<intptr_t, 0>::Run(meta, args); return MBoolCreate     (v); }
        case MType_Int    : { auto v = (int       )Caller<intptr_t, 0>::Run(meta, args); return MIntCreate      (v); }
        case MType_Float  : { auto v = (float     )Caller<float   , 0>::Run(meta, args); return MFloatCreate    (v); }
        case MType_C8Ptr  : { auto v = (char     *)Caller<intptr_t, 0>::Run(meta, args); return MStringCreateU8 (v); }
        case MType_C16Ptr : { auto v = (char16_t *)Caller<intptr_t, 0>::Run(meta, args); return MStringCreateU16(v); }
        case MType_MObject: { auto v = ( MObject *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MBool  : { auto v = ( MBool   *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MInt   : { auto v = ( MInt    *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MFloat : { auto v = ( MFloat  *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MString: { auto v = ( MString *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MLambda: { auto v = ( MLambda *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MData  : { auto v = ( MData   *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MArray : { auto v = ( MArray  *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MImage : { auto v = ( MImage  *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MNative: { auto v = (_MNative *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }

        default: return nullptr;
    }
}
