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

static intptr_t As_bool(MObject *object) {
    switch (MGetType(object)) {
        case MType_MBool : return (intptr_t)MBoolValue ((MBool  *)object);
        case MType_MInt  : return (intptr_t)MIntValue  ((MInt   *)object);
        case MType_MFloat: return (intptr_t)MFloatValue((MFloat *)object);

        default: return object != nullptr;
    }
}

static intptr_t As_int(MObject *object) {
    switch (MGetType(object)) {
        case MType_MBool : return (intptr_t)MBoolValue ((MBool  *)object);
        case MType_MInt  : return (intptr_t)MIntValue  ((MInt   *)object);
        case MType_MFloat: return (intptr_t)MFloatValue((MFloat *)object);

        default: return 0;
    }
}

static float As_float(MObject *object) {
    switch (MGetType(object)) {
        case MType_MBool : return (float)MBoolValue ((MBool  *)object);
        case MType_MInt  : return (float)MIntValue  ((MInt   *)object);
        case MType_MFloat: return (float)MFloatValue((MFloat *)object);

        default: return 0;
    }
}

static intptr_t As_pointer(MObject *object) {
    switch (MGetType(object)) {
        case MType_MPointer: return (intptr_t)MPointerValue ((MPointer *)object);
        case MType_MString : return (intptr_t)MStringU8Chars((MString  *)object);

        default: return 0;
    }
}

static intptr_t As_s8ptr(MObject *object) {
    if (MGetType(object) == MType_MString) {
        return (intptr_t)MStringU8Chars((MString *)object);
    }
    return 0;
}

static intptr_t As_s16ptr(MObject *object) {
    if (MGetType(object) == MType_MString) {
        return (intptr_t)MStringU16Chars((MString *)object);
    }
    return 0;
}

static intptr_t As_MObject(MObject *object) {
    return (intptr_t)object;
}

static intptr_t As_MBool   (MObject *a) { return MGetType(a) == MType_MBool    ? (intptr_t)a : 0; }
static intptr_t As_MInt    (MObject *a) { return MGetType(a) == MType_MInt     ? (intptr_t)a : 0; }
static intptr_t As_MFloat  (MObject *a) { return MGetType(a) == MType_MFloat   ? (intptr_t)a : 0; }
static intptr_t As_MPointer(MObject *a) { return MGetType(a) == MType_MPointer ? (intptr_t)a : 0; }
static intptr_t As_MString (MObject *a) { return MGetType(a) == MType_MString  ? (intptr_t)a : 0; }
static intptr_t As_MLambda (MObject *a) { return MGetType(a) == MType_MLambda  ? (intptr_t)a : 0; }
static intptr_t As_MData   (MObject *a) { return MGetType(a) == MType_MData    ? (intptr_t)a : 0; }
static intptr_t As_MArray  (MObject *a) { return MGetType(a) == MType_MArray   ? (intptr_t)a : 0; }
static intptr_t As_MImage  (MObject *a) { return MGetType(a) == MType_MImage   ? (intptr_t)a : 0; }
static intptr_t As_MSpecial(MObject *a) { return MGetType(a) == MType_MSpecial ? (intptr_t)a : 0; }

template<typename R, int N> struct Caller {

    template<typename... U> static R Run(const _MFuncMeta &meta, MArray *args, U... unfold) {

        if (N == meta.argCount) {
            return ((R (*)(U...))meta.address)(unfold...);
        }

        MObject *item = MArrayItem(args, N);
        MType type = meta.argTypes[N];

        switch (type) {
            case MType_bool    : return Caller<R, N + 1>::Run(meta, args, unfold..., As_bool    (item));
            case MType_int     : return Caller<R, N + 1>::Run(meta, args, unfold..., As_int     (item));
            case MType_float   : return Caller<R, N + 1>::Run(meta, args, unfold..., As_float   (item));
            case MType_pointer : return Caller<R, N + 1>::Run(meta, args, unfold..., As_pointer (item));
            case MType_s8ptr   : return Caller<R, N + 1>::Run(meta, args, unfold..., As_s8ptr   (item));
            case MType_s16ptr  : return Caller<R, N + 1>::Run(meta, args, unfold..., As_s16ptr  (item));
            case MType_MObject : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MObject (item));
            case MType_MBool   : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MBool   (item));
            case MType_MInt    : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MInt    (item));
            case MType_MFloat  : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MFloat  (item));
            case MType_MPointer: return Caller<R, N + 1>::Run(meta, args, unfold..., As_MPointer(item));
            case MType_MString : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MString (item));
            case MType_MLambda : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MLambda (item));
            case MType_MData   : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MData   (item));
            case MType_MArray  : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MArray  (item));
            case MType_MImage  : return Caller<R, N + 1>::Run(meta, args, unfold..., As_MImage  (item));
            case MType_MSpecial: return Caller<R, N + 1>::Run(meta, args, unfold..., As_MSpecial(item));

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
        case MType_void    : { /*  void return  */  Caller<void    , 0>::Run(meta, args); return nullptr            ; }
        case MType_bool    : { auto v = (bool      )Caller<intptr_t, 0>::Run(meta, args); return MBoolCreate     (v); }
        case MType_int     : { auto v = (int       )Caller<intptr_t, 0>::Run(meta, args); return MIntCreate      (v); }
        case MType_float   : { auto v = (float     )Caller<float   , 0>::Run(meta, args); return MFloatCreate    (v); }
        case MType_pointer : { auto v = (uint8_t  *)Caller<intptr_t, 0>::Run(meta, args); return MPointerCreate  (v); }
        case MType_s8ptr   : { auto v = (char     *)Caller<intptr_t, 0>::Run(meta, args); return MStringCreateU8 (v); }
        case MType_s16ptr  : { auto v = (char16_t *)Caller<intptr_t, 0>::Run(meta, args); return MStringCreateU16(v); }
        case MType_MObject : { auto v = (MObject  *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MBool   : { auto v = (MBool    *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MInt    : { auto v = (MInt     *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MFloat  : { auto v = (MFloat   *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MPointer: { auto v = (MPointer *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MString : { auto v = (MString  *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MLambda : { auto v = (MLambda  *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MData   : { auto v = (MData    *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MArray  : { auto v = (MArray   *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MImage  : { auto v = (MImage   *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }
        case MType_MSpecial: { auto v = (MSpecial *)Caller<intptr_t, 0>::Run(meta, args); return rr ? v : MRetain(v); }

        default: return nullptr;
    }
}
