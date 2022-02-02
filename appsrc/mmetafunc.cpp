#include "mmetafunc.h"

static def_singleton(sMetaMap , std::map<std::string, _MFuncMeta>());
static def_singleton(sIterator, std::map<std::string, _MFuncMeta>::iterator());

void _MFuncSetMeta(const char *name, _MFuncMeta meta, const char *note) {
    if (!name) {
        return;
    }

    //intentional memory leak.
    meta.funcName = MStringCreateU8(name);
    meta.funcNote = MStringCreateU8(note);

    sMetaMap()[name] = meta;
}

void MFuncSelectFirst() {
    sIterator() = sMetaMap().begin();
}

bool MFuncSelectedValid() {
    return sIterator() != sMetaMap().end();
}

void MFuncSelectNext() {
    ++sIterator();
}

bool MFuncSelect(const char *name) {
    if (name) {
        sIterator() = sMetaMap().find(name);
    } else {
        sIterator() = sMetaMap().end();
    }

    return sIterator() != sMetaMap().end();
}

MString *MFuncSelectedName     () { return sIterator()->second.funcName ; }
MString *MFuncSelectedNote     () { return sIterator()->second.funcNote ; }
MTypeId  MFuncSelectedRetTypeId() { return sIterator()->second.retTypeId; }
bool     MFuncSelectedRetRetain() { return sIterator()->second.retRetain; }
int      MFuncSelectedArgCount () { return sIterator()->second.argCount ; }

MTypeId MFuncSelectedArgTypeId(int index) {
    return sIterator()->second.argTypeIds[index];
}

static int64_t AsInt64(MObject *object) {
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MBool   *>::Value: return (int64_t)MBoolValue  ((MBool   *)object);
        case MTypeIdOf<MInt    *>::Value: return (int64_t)MIntValue   ((MInt    *)object);
        case MTypeIdOf<MInt64  *>::Value: return (int64_t)MInt64Value ((MInt64  *)object);
        case MTypeIdOf<MFloat  *>::Value: return (int64_t)MFloatValue ((MFloat  *)object);
        case MTypeIdOf<MDouble *>::Value: return (int64_t)MDoubleValue((MDouble *)object);

        default: return 0;
    }
}

static int AsInt(MObject *object) {
    return (int)AsInt64(object);
}

static double AsDouble(MObject *object) {
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MBool   *>::Value: return (double)MBoolValue  ((MBool   *)object);
        case MTypeIdOf<MInt    *>::Value: return (double)MIntValue   ((MInt    *)object);
        case MTypeIdOf<MInt64  *>::Value: return (double)MInt64Value ((MInt64  *)object);
        case MTypeIdOf<MFloat  *>::Value: return (double)MFloatValue ((MFloat  *)object);
        case MTypeIdOf<MDouble *>::Value: return /* .. */MDoubleValue((MDouble *)object);

        default: return 0;
    }
}

static float AsFloat(MObject *object) {
    return (float)AsDouble(object);
}

static bool AsBool(MObject *object) {
    auto value = (bool)AsInt64(object);
    return value ? true : object != nullptr;
}

static const void *AsPtr(MObject *object) {
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MString *>::Value: return MStringU8Chars((MString *)object);
        case MTypeIdOf<MPtr    *>::Value: return MPtrValue     ((MPtr    *)object);

        default: return 0;
    }
}

static const char *AsU8Ptr(MObject *object) {
    if (MGetTypeId(object) == MTypeIdOf<MString *>::Value) {
        return MStringU8Chars((MString *)object);
    }
    return 0;
}

static const char16_t *AsU16Ptr(MObject *object) {
    if (MGetTypeId(object) == MTypeIdOf<MString *>::Value) {
        return MStringU16Chars((MString *)object);
    }
    return 0;
}

static MObject *AsMObject(MObject *object) {
    return object;
}

//NOTE:
//template instantiation parameters only use 5 types: void, intptr_t, INT64_T, float and double,
//to prevent code bloat.

#if M_PTR_64
    typedef intptr_t INT64_T;
#else
    typedef int64_t  INT64_T;
#endif

template<typename R, int N> struct Caller {

    template<typename... U> static R Run(const _MFuncMeta &meta, MArray *args, U... unfold) {

        if (N == meta.argCount) {
            return ((R (*)(U...))meta.address)(unfold...);
        }

        MObject *ag = MArrayItem(args, N);
        MTypeId  id = meta.argTypeIds[N];

        if (id == MTypeIdOf<bool      >::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., (intptr_t)AsBool  (ag)); }
        if (id == MTypeIdOf<int       >::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., (intptr_t)AsInt   (ag)); }
        if (id == MTypeIdOf<int64_t   >::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., (INT64_T )AsInt64 (ag)); }
        if (id == MTypeIdOf<float     >::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., /* ...  */AsFloat (ag)); }
        if (id == MTypeIdOf<double    >::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., /* ...  */AsDouble(ag)); }
        if (id == MTypeIdOf<uint8_t  *>::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., (intptr_t)AsPtr   (ag)); }
        if (id == MTypeIdOf<char     *>::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., (intptr_t)AsU8Ptr (ag)); }
        if (id == MTypeIdOf<char16_t *>::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., (intptr_t)AsU16Ptr(ag)); }

        if (id == MTypeIdOf<MObject *>::Value || id == MGetTypeId(ag)) {
            return Caller<R, N + 1>::Run(meta, args, unfold..., (intptr_t)AsMObject(ag));
        } else {
            return Caller<R, N + 1>::Run(meta, args, unfold..., (intptr_t)0);
        }
    }
};

template<typename R> struct Caller<R, MFuncMaxArgCount> {

    template<typename... U> static R Run(const _MFuncMeta &meta, MArray *args, U... unfold) {
        return ((R (*)(U...))meta.address)(unfold...);
    }
};

MObject *MFuncCallCopyRet(MString *name, MArray *args) {
    if (!name) {
        return nullptr;
    }

    auto funcName = MStringU8Chars(name);
    auto iterator = sMetaMap().find(funcName);
    if (iterator == sMetaMap().end()) {
        return nullptr;
    }

    _MFuncMeta meta = iterator->second;
    MTypeId id = meta.retTypeId;
    
    if (id == MTypeIdOf<bool      >::Value) { return MBoolCreate     ((bool      )Caller<intptr_t, 0>::Run(meta, args)); }
    if (id == MTypeIdOf<int       >::Value) { return MIntCreate      ((int       )Caller<intptr_t, 0>::Run(meta, args)); }
    if (id == MTypeIdOf<int64_t   >::Value) { return MInt64Create    ((int64_t   )Caller<INT64_T , 0>::Run(meta, args)); }
    if (id == MTypeIdOf<float     >::Value) { return MFloatCreate    (/* ... .. */Caller<float   , 0>::Run(meta, args)); }
    if (id == MTypeIdOf<double    >::Value) { return MDoubleCreate   (/* ... .. */Caller<double  , 0>::Run(meta, args)); }
    if (id == MTypeIdOf<uint8_t  *>::Value) { return MPtrCreate      ((uint8_t  *)Caller<intptr_t, 0>::Run(meta, args)); }
    if (id == MTypeIdOf<char     *>::Value) { return MStringCreateU8 ((char     *)Caller<intptr_t, 0>::Run(meta, args)); }
    if (id == MTypeIdOf<char16_t *>::Value) { return MStringCreateU16((char16_t *)Caller<intptr_t, 0>::Run(meta, args)); }

    if (id == MTypeIdOf<void>::Value) {
        Caller<void, 0>::Run(meta, args);
        return nullptr;
    } else /* is a MObject */ {
        auto object = (MObject *)Caller<intptr_t, 0>::Run(meta, args);
        return meta.retRetain ? object : MRetain(object);
    }
}
