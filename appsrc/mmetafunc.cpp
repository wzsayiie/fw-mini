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

//NOTE:
//template instantiation parameters only use 3 types: void, float, and intptr_t,
//to prevent code bloat.
//

static intptr_t AsBool(MObject *object) {
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MBool  *>::Value: return (intptr_t)MBoolValue ((MBool  *)object);
        case MTypeIdOf<MInt   *>::Value: return (intptr_t)MIntValue  ((MInt   *)object);
        case MTypeIdOf<MFloat *>::Value: return (intptr_t)MFloatValue((MFloat *)object);

        default: return object != nullptr;
    }
}

static intptr_t AsInt(MObject *object) {
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MBool  *>::Value: return (intptr_t)MBoolValue ((MBool  *)object);
        case MTypeIdOf<MInt   *>::Value: return (intptr_t)MIntValue  ((MInt   *)object);
        case MTypeIdOf<MFloat *>::Value: return (intptr_t)MFloatValue((MFloat *)object);

        default: return 0;
    }
}

static float AsFloat(MObject *object) {
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MBool  *>::Value: return (float)MBoolValue ((MBool  *)object);
        case MTypeIdOf<MInt   *>::Value: return (float)MIntValue  ((MInt   *)object);
        case MTypeIdOf<MFloat *>::Value: return (float)MFloatValue((MFloat *)object);

        default: return 0;
    }
}

static intptr_t AsPointer(MObject *object) {
    switch (MGetTypeId(object)) {
        case MTypeIdOf<MPointer *>::Value: return (intptr_t)MPointerValue ((MPointer *)object);
        case MTypeIdOf<MString  *>::Value: return (intptr_t)MStringU8Chars((MString  *)object);

        default: return 0;
    }
}

static intptr_t AsS8Ptr(MObject *object) {
    if (MGetTypeId(object) == MTypeIdOf<MString *>::Value) {
        return (intptr_t)MStringU8Chars((MString *)object);
    }
    return 0;
}

static intptr_t AsS16Ptr(MObject *object) {
    if (MGetTypeId(object) == MTypeIdOf<MString *>::Value) {
        return (intptr_t)MStringU16Chars((MString *)object);
    }
    return 0;
}

static intptr_t AsMObject(MObject *object) {
    return (intptr_t)object;
}

template<typename R, int N> struct Caller {

    template<typename... U> static R Run(const _MFuncMeta &meta, MArray *args, U... unfold) {

        if (N == meta.argCount) {
            return ((R (*)(U...))meta.address)(unfold...);
        }

        MObject *item = MArrayItem(args, N);
        MTypeId id = meta.argTypeIds[N];

        if (id == MTypeIdOf<bool      >::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., AsBool   (item)); }
        if (id == MTypeIdOf<int       >::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., AsInt    (item)); }
        if (id == MTypeIdOf<float     >::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., AsFloat  (item)); }
        if (id == MTypeIdOf<uint8_t  *>::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., AsPointer(item)); }
        if (id == MTypeIdOf<char     *>::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., AsS8Ptr  (item)); }
        if (id == MTypeIdOf<char16_t *>::Value) { return Caller<R, N + 1>::Run(meta, args, unfold..., AsS16Ptr (item)); }

        if (id == MTypeIdOf<MObject *>::Value || id == MGetTypeId(item)) {
            return Caller<R, N + 1>::Run(meta, args, unfold..., AsMObject(item));
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
    if (id == MTypeIdOf<float     >::Value) { return MFloatCreate    ((float     )Caller<float   , 0>::Run(meta, args)); }
    if (id == MTypeIdOf<uint8_t  *>::Value) { return MPointerCreate  ((uint8_t  *)Caller<intptr_t, 0>::Run(meta, args)); }
    if (id == MTypeIdOf<char     *>::Value) { return MStringCreateU8 ((char     *)Caller<intptr_t, 0>::Run(meta, args)); }
    if (id == MTypeIdOf<char16_t *>::Value) { return MStringCreateU16((char16_t *)Caller<intptr_t, 0>::Run(meta, args)); }

    if (id == MTypeIdOf<void>::Value) {
        Caller<void, 0>::Run(meta, args);
        return nullptr;
    } else {
        //must be a MObject.
        auto object = (MObject *)Caller<intptr_t, 0>::Run(meta, args);
        return meta.retRetain ? object : MRetain(object);
    }
}
