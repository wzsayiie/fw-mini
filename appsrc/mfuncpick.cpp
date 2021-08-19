#include <cstring>

//define collection macro:

#define MFUNC_META(name) ; static int _unused_##name = (SetFunc(#name, name), 0)
#include "mconfig.h"

//TypeOf:

template<typename T> struct TypeOf;

template<> struct TypeOf<void            > { static const MType Value = MType_Void   ; };
template<> struct TypeOf<bool            > { static const MType Value = MType_Bool   ; };
template<> struct TypeOf<int             > { static const MType Value = MType_Int    ; };
template<> struct TypeOf<float           > { static const MType Value = MType_Float  ; };
template<> struct TypeOf<char           *> { static const MType Value = MType_C8Ptr  ; };
template<> struct TypeOf<const char     *> { static const MType Value = MType_C8Ptr  ; };
template<> struct TypeOf<char16_t       *> { static const MType Value = MType_C16Ptr ; };
template<> struct TypeOf<const char16_t *> { static const MType Value = MType_C16Ptr ; };
template<> struct TypeOf<class  MObject *> { static const MType Value = MType_MObject; };
template<> struct TypeOf<class  MBool   *> { static const MType Value = MType_MBool  ; };
template<> struct TypeOf<class  MInt    *> { static const MType Value = MType_MInt   ; };
template<> struct TypeOf<class  MFloat  *> { static const MType Value = MType_MFloat ; };
template<> struct TypeOf<class  MString *> { static const MType Value = MType_MString; };
template<> struct TypeOf<class  MLambda *> { static const MType Value = MType_MLambda; };
template<> struct TypeOf<class  MData   *> { static const MType Value = MType_MData  ; };
template<> struct TypeOf<class  MArray  *> { static const MType Value = MType_MArray ; };
template<> struct TypeOf<class  MImage  *> { static const MType Value = MType_MImage ; };
template<> struct TypeOf<class _MNative *> { static const MType Value = MType_MNative; };

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
    meta->argTypes[(meta->argCount)++] = TypeOf<A>::Value;
    AppendArgs(meta, (R (*)(B...))nullptr);
}

//collect functions:

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

template<typename R, typename... A> void SetFunc(const char *name, R (*func)(A...)) {

    _MFuncMeta meta;

    meta.address   = (void *)func;
    meta.retType   = TypeOf<R>::Value;
    meta.retRetain = strstr(name, "Create") || strstr(name, "Retain") || strstr(name, "Copy");

    static_assert((ArgCountOf<R (A...)>::Value) <= MFuncMaxArgCount, "");
    AppendArgs(&meta, func);

    _MFuncSetMeta(name, meta);
}

#include "minikit.h"
