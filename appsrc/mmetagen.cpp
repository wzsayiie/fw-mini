#include <cstring>

//collection macro:

#define M_META(name, ...) ; static int _unused_##name = (Collect(#name, name, ##__VA_ARGS__), 0)
#include "mconvention.h"

//collect constants:

void _MConstSetU8Chars (const char *name, const char     *value, const char *note);
void _MConstSetU16Chars(const char *name, const char16_t *value, const char *note);
void _MConstSetInt     (const char *name, int             value, const char *note);
void _MConstSetFloat   (const char *name, float           value, const char *note);

void Collect(const char *n, const char     *v, const char *a = nullptr) { _MConstSetU8Chars (n, v, a); }
void Collect(const char *n, const char16_t *v, const char *a = nullptr) { _MConstSetU16Chars(n, v, a); }
void Collect(const char *n, int             v, const char *a = nullptr) { _MConstSetInt     (n, v, a); }
void Collect(const char *n, float           v, const char *a = nullptr) { _MConstSetFloat   (n, v, a); }

//collect functions:

void _MFuncSetMeta(const char *name, _MFuncMeta meta, const char *note);

template<typename T> struct ArgCount;
template<typename R> struct ArgCount<R ()> {
    static const int Value = 0;
};
template<typename R, typename A, typename... B> struct ArgCount<R (A, B...)> {
    static const int Value = 1 + ArgCount<R (B...)>::Value;
};

template<typename R> void ScanArgs(_MFuncMeta *, R (*)()) {
}
template<typename R, typename A, typename... B> void ScanArgs(_MFuncMeta *meta, R (*)(A, B...)) {
    meta->argTypeIds[(meta->argCount)++] = MTypeIdOf<A>::Value;
    ScanArgs(meta, (R (*)(B...))nullptr);
}

template<typename R, typename... A> void Collect(
    const char *name, R (*func)(A...), const char *note = nullptr)
{
    _MFuncMeta meta;

    meta.address   = (void *)func;
    meta.retTypeId = MTypeIdOf<R>::Value;
    meta.retRetain = strstr(name, "Create") || strstr(name, "Retain") || strstr(name, "Copy");

    static_assert((ArgCount<R (A...)>::Value) <= MFuncMaxArgCount);
    ScanArgs(&meta, func);

    _MFuncSetMeta(name, meta, note);
}

#include "minikit.h"
