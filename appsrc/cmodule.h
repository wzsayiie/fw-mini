#pragma once

#include "minikit.h"

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

struct _CModVPtr {
    //for clang and gcc, the virtual function pointer occupies two words,
    //and only one under ms cl.
    void *value[2] = {0};
};

template<typename R, typename C, typename... A> _CModVPtr _CModGetVPtr(R (C::*src)(A...)) {
    _CModVPtr dst;
    memcpy(&dst, &src, sizeof(src));
    return dst;
}

const int _CModMethodMaxArgCount = 2;

struct _CModMetaCommitment {
    const cmod_char *intfName     = nullptr;
    const cmod_char *baseIntfName = nullptr;
    void *(*createShellObj)()     = nullptr;

    const cmod_char *methodName = nullptr;
    void            *equalFunc  = nullptr;
    _CModVPtr        methodVPtr = {0};
    const cmod_char *retType    = nullptr;
    bool             retRetain  = false;
    int              argCount   = 0;

    const cmod_char *argTypes[_CModMethodMaxArgCount] = {0};
};

void _CModCommitMeta(_CModMetaCommitment *commitment);

template<typename T> struct _CModBaseIntfName;

bool _CModIsRetRetain(const cmod_char *methodName);

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
    R (*equalFunc)(C *, _CModVPtr *, A...) = [](C *obj, _CModVPtr *vptr, A... args) {
        using vtype = R (C::*)(A...);
        auto method = *(vtype *)vptr;
        return (obj->*method)(args...);
    };

    _CModMetaCommitment commitment; {
        commitment.intfName       = intfName;
        commitment.baseIntfName   = _CModBaseIntfName<C>::Value;
        commitment.createShellObj = []() -> void * { return new C(); };

        commitment.methodName = methodName;
        commitment.equalFunc  = (void *)equalFunc;
        commitment.methodVPtr = _CModGetVPtr(ptr);
        commitment.retType    = _CModType<R>::Value;
        commitment.retRetain  = _CModIsRetRetain(methodName);

        static_assert(_CModArgCount<R (A...)>::Value <= _CModMethodMaxArgCount);
        _CModScanArgs(&commitment, (R (*)(A...))nullptr);
    }

    _CModCommitMeta(&commitment);

    return 0;
}

//use followed functions to access interface metadata:
//

struct CModIntf;
struct CModMethod;

M_FUNC_EXPORT int       CModIntfCount();
M_FUNC_EXPORT CModIntf *CModIntfEntry(int intfIndex);
M_FUNC_EXPORT CModIntf *CModIntfFind (const cmod_char *intfName);

M_FUNC_EXPORT const cmod_char *CModIntfName          (CModIntf *intf);
M_FUNC_EXPORT bool             CModIntfAssignable    (CModIntf *intf, CModIntf *target);
M_FUNC_EXPORT void            *CModIntfCreateShellObj(CModIntf *intf);
M_FUNC_EXPORT CModIntf        *CModIntfBase          (CModIntf *intf);

M_FUNC_EXPORT int         CModMethodCount(CModIntf *intf);
M_FUNC_EXPORT CModMethod *CModMethodEntry(CModIntf *intf, int methodIndex);
M_FUNC_EXPORT CModMethod *CModMethodFind (CModIntf *intf, const cmod_char *methodName);

M_FUNC_EXPORT const cmod_char *CModMethodName     (CModMethod *method);
M_FUNC_EXPORT void            *CModMethodEqualFunc(CModMethod *method);
M_FUNC_EXPORT _CModVPtr       *CModMethodVPtr     (CModMethod *method);
M_FUNC_EXPORT const cmod_char *CModMethodRetType  (CModMethod *method);
M_FUNC_EXPORT bool             CModMethodRetRetain(CModMethod *method);
M_FUNC_EXPORT int              CModMethodArgCount (CModMethod *method);
M_FUNC_EXPORT const cmod_char *CModMethodArgType  (CModMethod *method, int argIndex);

//use "IModObj", "cmod_intf" and "CMOD_META" to define a interface:
//

template<> struct _CModBaseIntfName<struct IModObj> {
    static constexpr const cmod_char *const Value = nullptr;
};
template<> struct _CModType<struct IModObj *> {
    static constexpr const cmod_char *const Value = CMOD_L "IModObj";
};
struct IModObj {
    virtual ~IModObj();

    virtual const cmod_char *intfName();
    virtual IModObj *retain();
    virtual void release();
    virtual void destroy();
    
protected:
    void *mInjectedTab = nullptr;
    void *mInjectedObj = nullptr;

private:
    int mRefCount = 1;
};

struct _CModMakeSharedHelper {
    template<typename T> std::shared_ptr<T> operator<<(T *modObj) {
        if (modObj) {
            modObj->retain();
            return std::shared_ptr<T>(modObj, [](T *obj) { obj->release(); });
        } else {
            return std::shared_ptr<T>();
        }
    }
};
#define cmod_make_shared _CModMakeSharedHelper()<<

struct _CModAutoReleaseHelper {
    template<typename T> std::shared_ptr<T> operator<<(T *modObj) {
        if (modObj) {
            return std::shared_ptr<T>(modObj, [](T *obj) { obj->release(); });
        } else {
            return std::shared_ptr<T>();
        }
    }
};
#define cmod_auto_release _CModAutoReleaseHelper()<<

#define cmod_intf(name, base)                                               \
/**/    template<> struct _CModBaseIntfName<struct name> {                  \
/**/        static constexpr const cmod_char *const Value = CMOD_L #base;   \
/**/    };                                                                  \
/**/    template<> struct _CModType<struct name *> {                        \
/**/        static constexpr const cmod_char *const Value = CMOD_L #name;   \
/**/    };                                                                  \
/**/    struct _middle_##name : base {                                      \
/**/        const cmod_char *intfName() override {                          \
/**/            return CMOD_L #name;                                        \
/**/        }                                                               \
/**/    };                                                                  \
/**/    struct name : _middle_##name

#define CMOD_META(ret, intf, method, args, ...)                                             \
/**/    ret intf::method args {                                                             \
/**/        auto func = _CModCastInjectedFunc(&intf::method, mInjectedTab, CMOD_L #method); \
/**/        if (func) {                                                                     \
/**/            return func(mInjectedObj, ##__VA_ARGS__);                                   \
/**/        } else {                                                                        \
/**/            return _CModDefaultReturn(&intf::method);                                   \
/**/        }                                                                               \
/**/    };                                                                                  \
/**/    _CMOD_COLLECT_META(intf, method)

void *_CModGetInjectedFunc(void *injectedTab, const cmod_char *methodName);

template<typename R, typename C, typename... A> auto _CModCastInjectedFunc(
    R (C::*)(A...), void *injectedTab, const cmod_char *methodName) -> R (*)(void *, A...)
{
    return (R (*)(void *, A...))_CModGetInjectedFunc(injectedTab, methodName);
}

template<typename R, typename C, typename... A> R _CModDefaultReturn(R (C::*)(A...)) {
    return R();
}

//call a object on runtime:
//

struct CModPass;

M_FUNC_EXPORT CModPass *CModPassCreate();
M_FUNC_EXPORT void CModPassRelease(CModPass *pass);

M_FUNC_EXPORT void CModPassPushBool  (CModPass *pass, bool             value);
M_FUNC_EXPORT void CModPassPushInt   (CModPass *pass, int              value);
M_FUNC_EXPORT void CModPassPushInt64 (CModPass *pass, int64_t          value);
M_FUNC_EXPORT void CModPassPushFloat (CModPass *pass, float            value);
M_FUNC_EXPORT void CModPassPushDouble(CModPass *pass, double           value);
M_FUNC_EXPORT void CModPassPushIntPtr(CModPass *pass, const void      *value);
M_FUNC_EXPORT void CModPassPushChrPtr(CModPass *pass, const cmod_char *value);
M_FUNC_EXPORT void CModPassPushObj   (CModPass *pass, IModObj         *value);

M_FUNC_EXPORT bool CModPassCall(CModPass *pass, IModObj *obj, const cmod_char *methodName);

M_FUNC_EXPORT bool       CModPassRetBool  (CModPass *pass);
M_FUNC_EXPORT int        CModPassRetInt   (CModPass *pass);
M_FUNC_EXPORT int64_t    CModPassRetInt64 (CModPass *pass);
M_FUNC_EXPORT float      CModPassRetFloat (CModPass *pass);
M_FUNC_EXPORT double     CModPassRetDouble(CModPass *pass);
M_FUNC_EXPORT void      *CModPassRetIntPtr(CModPass *pass);
M_FUNC_EXPORT cmod_char *CModPassRetChrPtr(CModPass *pass);
M_FUNC_EXPORT IModObj   *CModPassRetainRet(CModPass *pass);

//use followed functions to implement a interface on runtime:
//

struct CModCls;

M_FUNC_EXPORT CModCls *CModClsImplement(const cmod_char *intfName);
M_FUNC_EXPORT void     CModDefineMethod(CModCls *cls, const cmod_char *methodName, void *func);
M_FUNC_EXPORT IModObj *CModCreateObj   (CModCls *cls, void *injectedObj);
