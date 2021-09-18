#include <cstring>

//collection macro:

#define M_META(name) ; static int _unused_##name = (Collect(#name, name), 0)
#include "mconfig.h"

//collect constants;

void _MConstSetU8Chars (const char *name, const char     *value);
void _MConstSetU16Chars(const char *name, const char16_t *value);
void _MConstSetInt     (const char *name, int             value);
void _MConstSetFloat   (const char *name, float           value);

/* static */ void Collect(const char *name, const char     *value) { _MConstSetU8Chars (name, value); }
/* static */ void Collect(const char *name, const char16_t *value) { _MConstSetU16Chars(name, value); }
/* static */ void Collect(const char *name, int             value) { _MConstSetInt     (name, value); }
/* static */ void Collect(const char *name, float           value) { _MConstSetFloat   (name, value); }

//collect functions:

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

template<typename T> struct ArgCountOf;
template<typename R> struct ArgCountOf<R ()> {
    static const int Value = 0;
};
template<typename R, typename A, typename... B> struct ArgCountOf<R (A, B...)> {
    static const int Value = 1 + ArgCountOf<R (B...)>::Value;
};

template<typename R> void AppendArgs(_MFuncMeta *meta, R (*)()) {
}
template<typename R, typename A, typename... B> void AppendArgs(_MFuncMeta *meta, R (*)(A, B...)) {
    meta->argTypeIds[(meta->argCount)++] = MTypeIdOf<A>::Value;
    AppendArgs(meta, (R (*)(B...))nullptr);
}

template<typename R, typename... A> void Collect(const char *name, R (*func)(A...)) {

    _MFuncMeta meta;

    meta.address   = (void *)func;
    meta.retTypeId = MTypeIdOf<R>::Value;
    meta.retRetain = strstr(name, "Create") || strstr(name, "Retain") || strstr(name, "Copy");

    static_assert((ArgCountOf<R (A...)>::Value) <= MFuncMaxArgCount, "");
    AppendArgs(&meta, func);

    _MFuncSetMeta(name, meta);
}

#include "minikit.h"
