#include <cstdint>
#include <cstring>

//collection macro:

#define MFUNC_META(name) ; static int _unused_##name = (SetFunc(#name, name), 0)
#include "mconfig.h"

//ArgCountOf:

template<typename T> struct ArgCountOf;

template<typename R> struct ArgCountOf<R ()> {
    static const int Value = 0;
};

template<typename R, typename A, typename... B> struct ArgCountOf<R (A, B...)> {
    static const int Value = 1 + ArgCountOf<R (B...)>::Value;
};

//AppendArgs:

template<typename R> void AppendArgs(_MFuncMeta *meta, R (*)()) {
}

template<typename R, typename A, typename... B> void AppendArgs(_MFuncMeta *meta, R (*)(A, B...)) {
    meta->argTypeIds[(meta->argCount)++] = MTypeIdOf<A>::Value;
    AppendArgs(meta, (R (*)(B...))nullptr);
}

//collect functions:

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

template<typename R, typename... A> void SetFunc(const char *name, R (*func)(A...)) {

    _MFuncMeta meta;

    meta.address   = (void *)func;
    meta.retTypeId = MTypeIdOf<R>::Value;
    meta.retRetain = strstr(name, "Create") || strstr(name, "Retain") || strstr(name, "Copy");

    static_assert((ArgCountOf<R (A...)>::Value) <= MFuncMaxArgCount, "");
    AppendArgs(&meta, func);

    _MFuncSetMeta(name, meta);
}

#include "minikit.h"
