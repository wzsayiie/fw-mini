#include <cstdint>
#include <cstring>

#define MFUNC_META(name) ; static int _unused_##name = (Pick(#name, name), 0)
#include "mconfig.h"

template<typename T> struct Extr;

template<> struct Extr<void            > { static const MType T = MType_Void   ; };
template<> struct Extr<bool            > { static const MType T = MType_Bool   ; };
template<> struct Extr<int             > { static const MType T = MType_Int    ; };
template<> struct Extr<float           > { static const MType T = MType_Float  ; };
template<> struct Extr<uint8_t        *> { static const MType T = MType_C8Ptr  ; };
template<> struct Extr<const uint8_t  *> { static const MType T = MType_C8Ptr  ; };
template<> struct Extr<char           *> { static const MType T = MType_C8Ptr  ; };
template<> struct Extr<const char     *> { static const MType T = MType_C8Ptr  ; };
template<> struct Extr<char16_t       *> { static const MType T = MType_C16Ptr ; };
template<> struct Extr<const char16_t *> { static const MType T = MType_C16Ptr ; };
template<> struct Extr<class MObject  *> { static const MType T = MType_MObject; };
template<> struct Extr<class MBool    *> { static const MType T = MType_MBool  ; };
template<> struct Extr<class MInt     *> { static const MType T = MType_MInt   ; };
template<> struct Extr<class MFloat   *> { static const MType T = MType_MFloat ; };
template<> struct Extr<class MString  *> { static const MType T = MType_MString; };
template<> struct Extr<class MLambda  *> { static const MType T = MType_MLambda; };
template<> struct Extr<class MData    *> { static const MType T = MType_MData  ; };
template<> struct Extr<class MArray   *> { static const MType T = MType_MArray ; };
template<> struct Extr<class MImage   *> { static const MType T = MType_MImage ; };
template<> struct Extr<class _MNative *> { static const MType T = MType_MNative; };

void _MFuncSetMeta(const char *name, const _MFuncMeta &meta);

template<typename R> void Set(const char* name, void *func, int argc, MType a, MType b, MType c, MType d) {
    _MFuncMeta meta;
    
    meta.address   = func;
    meta.retType   = Extr<R>::T;
    meta.retRetain = strstr(name, "Create") || strstr(name, "Retain") || strstr(name, "Copy");
    meta.argCount  = argc;
    meta.argType0  = a;
    meta.argType1  = b;
    meta.argType2  = c;
    meta.argType3  = d;

    _MFuncSetMeta(name, meta);
}

#define tpl_0 template<class R>
#define tpl_1 template<class R, class A>
#define tpl_2 template<class R, class A, class B>
#define tpl_3 template<class R, class A, class B, class C>
#define tpl_4 template<class R, class A, class B, class C, class D>

tpl_0 void Pick(const char* n, R (*f)(          )) { Set<R>(n, f, 0, 0         , 0         , 0         , 0         ); }
tpl_1 void Pick(const char* n, R (*f)(A         )) { Set<R>(n, f, 1, Extr<A>::T, 0         , 0         , 0         ); }
tpl_2 void Pick(const char* n, R (*f)(A, B      )) { Set<R>(n, f, 2, Extr<A>::T, Extr<B>::T, 0         , 0         ); }
tpl_3 void Pick(const char* n, R (*f)(A, B, C   )) { Set<R>(n, f, 3, Extr<A>::T, Extr<B>::T, Extr<C>::T, 0         ); }
tpl_4 void Pick(const char* n, R (*f)(A, B, C, D)) { Set<R>(n, f, 4, Extr<A>::T, Extr<B>::T, Extr<C>::T, Extr<D>::T); }

#include "minikit.h"
