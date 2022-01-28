#pragma once

#include <cstdint>
#include <string>

//the character set used by modules:
//

#if _CMOD_USE_UTF16
    typedef char16_t       cmod_char;
    typedef std::u16string cmod_string;
    #define CMOD_L         u""
#elif _CMOD_USE_UTF8
    typedef char           cmod_char;
    typedef std::string    cmod_string;
    #define CMOD_L         u8""
#else //ascii.
    typedef char           cmod_char;
    typedef std::string    cmod_string;
    #define CMOD_L         ""
#endif

//basic data types that modules can use:
//

template<typename T> struct _CModType;

#define _CModT static constexpr const cmod_char *const

template<> struct _CModType<void   > { _CModT Value = CMOD_L "void"  ; };
template<> struct _CModType<bool   > { _CModT Value = CMOD_L "bool"  ; };
template<> struct _CModType<int    > { _CModT Value = CMOD_L "int"   ; };
template<> struct _CModType<int64_t> { _CModT Value = CMOD_L "int64" ; };
template<> struct _CModType<float  > { _CModT Value = CMOD_L "float" ; };
template<> struct _CModType<double > { _CModT Value = CMOD_L "double"; };

template<> struct _CModType<const void      *> { _CModT Value = CMOD_L "intptr"; };
template<> struct _CModType<void            *> { _CModT Value = CMOD_L "intptr"; };
template<> struct _CModType<const cmod_char *> { _CModT Value = CMOD_L "chrptr"; };
template<> struct _CModType<cmod_char       *> { _CModT Value = CMOD_L "chrptr"; };

#undef _CModT

//collect interface metadata with "_CMOD_COLLECT_META":
//

#define _CMOD_COLLECT_META(intf, method) static int _unused_##intf##_##method = \
/**/    _CModCollectMeta(CMOD_L #intf, CMOD_L #method, &intf::method)

const int _CModMethodMaxArgCount = 4;

struct _CModMetaCommitment {
    const cmod_char *intfName = nullptr;
    void *(*createNilObj)()   = nullptr;

    const cmod_char *methodName = nullptr;
    int              vtabIndex  = 0;
    void            *equalFunc  = nullptr;
    const cmod_char *retType    = nullptr;
    int              argCount   = 0;

    const cmod_char *argTypes[_CModMethodMaxArgCount] = {0};
};

void _CModCommitMeta(_CModMetaCommitment *commitment);

template<typename R, typename C, typename... A> int _CModGetVTabIndex(R (C::*ptr)(A...)) {
    auto offset = *(intptr_t *)&ptr;
#if _MSC_VER
    return (int)((offset    ) / sizeof(void *));
#else
    return (int)((offset - 1) / sizeof(void *));
#endif
}

template<typename T> struct _CModArgCount;
template<typename R, typename A, typename... B> struct _CModArgCount<R (A, B...)> {
    static const int Value = 1 + _CModArgCount<R (B...)>::Value;
};
template<typename R> struct _CModArgCount<R ()> {
    static const int Value = 0;
};

template<typename R> void _CModScanArgs(_CModMetaCommitment *, R (*)()) {
}
template<typename R, typename A, typename... B> void _CModScanArgs(
    _CModMetaCommitment *commitment, R (*)(A, B...))
{
    commitment->argTypes[(commitment->argCount)++] = _CModType<A>::Value;
    _CModScanArgs(commitment, (R (*)(B...))nullptr);
}

template<typename R, typename C, typename... A> int _CModCollectMeta(
    const cmod_char *intfName, const cmod_char *methodName, R (C::*ptr)(A...))
{
    R (*equalFunc)(C *a, R (C::*m)(A...), A... s) = [](C *a, R (C::*m)(A...), A... s) {
        return (a->*m)(s...);
    };

    _CModMetaCommitment commitment; {
        commitment.intfName     = intfName;
        commitment.createNilObj = []() -> void * { return new C(); };

        commitment.methodName = methodName;
        commitment.vtabIndex  = _CModGetVTabIndex(ptr);
        commitment.equalFunc  = (void *)equalFunc;
        commitment.retType    = _CModType<R>::Value;

        static_assert(_CModArgCount<R (A...)>::Value <= _CModMethodMaxArgCount);
        _CModScanArgs(&commitment, (R (*)(A...))nullptr);
    }

    _CModCommitMeta(&commitment);

    return 0;
}

//use followed functions to access interface metadata:
//

#if _MSC_VER
    #define CMOD_FUNC extern "C" __declspec(dllexport)
#else
    #define CMOD_FUNC extern "C" __attribute((visibility("default")))
#endif

struct CModIntf;
struct CModMethod;

CMOD_FUNC int       CModIntfCount();
CMOD_FUNC CModIntf *CModIntfEntry(int intfIndex);
CMOD_FUNC CModIntf *CModIntfFind (const cmod_char *intfName);

CMOD_FUNC const cmod_char *CModIntfName(CModIntf *intf);
CMOD_FUNC void *CModIntfCreateNilObj(CModIntf *intf);
CMOD_FUNC int CModIntfVTabSize(CModIntf *intf);

CMOD_FUNC int         CModMethodCount(CModIntf *intf);
CMOD_FUNC CModMethod *CModMethodEntry(CModIntf *intf, int methodIndex);
CMOD_FUNC CModMethod *CModMethodFind (CModIntf *intf, const cmod_char *methodName);

CMOD_FUNC const cmod_char *CModMethodName     (CModMethod *method);
CMOD_FUNC int              CModMethodVTabIndex(CModMethod *method);
CMOD_FUNC void            *CModMethodEqualFunc(CModMethod *method);
CMOD_FUNC const cmod_char *CModMethodRetType  (CModMethod *method);
CMOD_FUNC int              CModMethodArgCount (CModMethod *method);
CMOD_FUNC const cmod_char *CModMethodArgType  (CModMethod *method, int argIndex);

//use "IModObj", "cmod_intf" and "CMOD_META" to define a interface:
//

template<> struct _CModType<struct IModObj *> {
    static constexpr const cmod_char *const Value = CMOD_L "IModObj";
};
struct IModObj {
    virtual ~IModObj();

    virtual const cmod_char *intfName();
    virtual IModObj *retain();
    virtual void release();
    virtual void destroy();

private:
    int mRefCount = 1;

protected:
    void **mInjectedTab = nullptr;
    void  *mPayloadObj  = nullptr;
};

#define cmod_intf(name, base)                                               \
/**/    template<> struct _CModType<struct name *> {                        \
/**/        static constexpr const cmod_char *const Value = CMOD_L #name;   \
/**/    };                                                                  \
/**/    struct _middle_##name : base {                                      \
/**/        const cmod_char *intfName() override {                          \
/**/            return CMOD_L #name;                                        \
/**/        }                                                               \
/**/    };                                                                  \
/**/    struct name : base

#define CMOD_META(ret, intf, method, args, ...)                             \
/**/    ret intf::method args {                                             \
/**/        auto func = _CModGetInjectedFunc(&intf::method, mInjectedTab);  \
/**/        if (func) {                                                     \
/**/            return func(mPayloadObj, ##__VA_ARGS__);                    \
/**/        } else {                                                        \
/**/            return _CModDefaultReturn(&intf::method);                   \
/**/        }                                                               \
/**/    };                                                                  \
/**/    _CMOD_COLLECT_META(intf, method)

template<typename R, typename C, typename... A> auto _CModGetInjectedFunc(
    R (C::*method)(A...), void **injectedTab) -> R (*)(void *, A...)
{
    if (injectedTab) {
        int index = _CModGetVTabIndex(method);
        return (R (*)(void *, A...))injectedTab[index];
    }
    return nullptr;
}

template<typename R, typename C, typename... A> R _CModDefaultReturn(R (C::*)(A...)) {
    return R();
}

//call a object on runtime:
//

//use followed functions to implement a interface on runtime:
//

struct CModCls;

CMOD_FUNC CModCls *CModClsImplement(const cmod_char *intfName);
CMOD_FUNC void     CModDefineMethod(CModCls *cls, const cmod_char *methodName, void *func);
CMOD_FUNC IModObj *CModCreateObj   (CModCls *cls, void *loadObj);
