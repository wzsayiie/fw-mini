#include "cmodmeta.h"
#include <algorithm>
#include <map>
#include <vector>

//string assistant functions:
//

static const cmod_char *cmod_stristr(const cmod_char *str, const cmod_char *aim) {
    cmod_string s = str;
    cmod_string a = aim;
    
    std::transform(s.begin(), s.end(), s.begin(), std::tolower);
    std::transform(a.begin(), a.end(), a.begin(), std::tolower);
    
    auto pos = s.find(a);
    return pos != cmod_string::npos ? str + pos : nullptr;
}

static int cmod_strcmp(const cmod_char *a, const cmod_char *b) {
    for (; *a == *b; ++a, ++b) {
        if (*a == '\0') {
            return 0;
        }
    }
    return *a < *b ? -1 : 1;
}

//interface metadata store:
//

struct CModMethod : _CModMetaCommitment {
};

struct CModIntf {
    const cmod_char *intfName     = nullptr;
    const cmod_char *baseIntfName = nullptr;
    void *(*createShellObj)()     = nullptr;

    std::map<cmod_string, CModMethod *> methodMap;
    std::vector<CModMethod *> methodList;
};

struct MetaStore {
    std::map<cmod_string, CModIntf *> intfMap;
    std::vector<CModIntf *> intfList;
};

static MetaStore *GetMetaStore() {
    static auto store = new MetaStore();
    return store;
}

static CModIntf *GetIntf(const cmod_char *intfName, bool force) {
    if (!intfName) {
        return nullptr;
    }

    MetaStore *store = GetMetaStore();

    auto target = store->intfMap.find(intfName);
    if (target != store->intfMap.end()) {
        return target->second;
    }

    if (!force) {
        return nullptr;
    }

    //intentional leak.
    auto intf = new CModIntf();
    intf->intfName = intfName;

    store->intfMap[intfName] = intf;
    store->intfList.push_back(intf);

    return intf;
}

void _CModCommitMeta(_CModMetaCommitment *commitment) {
    CModIntf *intf = GetIntf(commitment->intfName, true);
    
    //NOTE: method overloading is unsupported.
    auto existing = intf->methodMap.find(commitment->methodName);
    if (existing != intf->methodMap.end()) {
        return;
    }

    //intentional leak.
    auto method = new CModMethod();
    *(_CModMetaCommitment *)method = *commitment;

    intf->baseIntfName   = commitment->baseIntfName;
    intf->createShellObj = commitment->createShellObj;
    intf->methodMap[commitment->methodName] = method;
    intf->methodList.push_back(method);
}

bool _CModIsRetRetain(const cmod_char *methodName) {
    return (
        !cmod_stristr(methodName, CMOD_L "create") &&
        !cmod_stristr(methodName, CMOD_L "copy"  ) &&
        !cmod_stristr(methodName, CMOD_L "retain")
    );
}

//access interface meta:
//

int CModIntfCount() {
    return (int)GetMetaStore()->intfList.size();
}

CModIntf *CModIntfEntry(int intfIndex) {
    std::vector<CModIntf *> *list = &GetMetaStore()->intfList;
    if (0 <= intfIndex && intfIndex < (int)list->size()) {
        return list->at(intfIndex);
    }
    return nullptr;
}

CModIntf *CModIntfFind(const cmod_char *intfName) {
    return GetIntf(intfName, false);
}

const cmod_char *CModIntfName(CModIntf *intf) {
    if (intf) {
        return intf->intfName;
    }
    return nullptr;
}

bool CModIntfAssignable(CModIntf *intf, CModIntf *target) {
    if (!intf || !target) {
        return false;
    }
    
    for (; intf; intf = CModIntfBase(intf)) {
        if (intf == target) {
            return true;
        }
    }
    return false;
}

void *CModIntfCreateShellObj(CModIntf *intf) {
    if (intf) {
        return intf->createShellObj();
    }
    return nullptr;
}

CModIntf *CModIntfBase(CModIntf *intf) {
    if (intf) {
        return GetIntf(intf->baseIntfName, false);
    }
    return nullptr;
}

int CModMethodCount(CModIntf *intf) {
    if (intf) {
        return (int)intf->methodList.size();
    }
    return 0;
}

CModMethod *CModMethodEntry(CModIntf *intf, int methodIndex) {
    if (!intf) {
        return nullptr;
    }

    std::vector<CModMethod *> *list = &intf->methodList;
    if (0 <= methodIndex && methodIndex < (int)list->size()) {
        return list->at(methodIndex);
    }
    return nullptr;
}

CModMethod *CModMethodFind(CModIntf *intf, const cmod_char *methodName) {
    if (!intf) {
        return nullptr;
    }
    if (!methodName) {
        return nullptr;
    }

    auto target = intf->methodMap.find(methodName);
    if (target == intf->methodMap.end()) {
        return nullptr;
    }

    return target->second;
}

const cmod_char *CModMethodName     (CModMethod *m) { return m ? m->methodName : nullptr; }
void            *CModMethodEqualFunc(CModMethod *m) { return m ? m->equalFunc  : nullptr; }
_CModVPtr       *CModMethodVPtr     (CModMethod *m) { return m ?&m->methodVPtr : nullptr; }
const cmod_char *CModMethodRetType  (CModMethod *m) { return m ? m->retType    : nullptr; }
bool            CModMethodRetRetain (CModMethod *m) { return m ? m->retRetain  : false  ; }
int             CModMethodArgCount  (CModMethod *m) { return m ? m->argCount   : 0      ; }

const cmod_char *CModMethodArgType(CModMethod *method, int argIndex) {
    if (!method) {
        return nullptr;
    }
    
    if (0 <= argIndex && argIndex < method->argCount) {
        return method->argTypes[argIndex];
    }
    return nullptr;
}

//IModObj, the base of all interfaces:
//

IModObj::~IModObj() {
}

const cmod_char *IModObj::intfName() {
    return _CModType<IModObj *>::Value;
}

IModObj *IModObj::retain() {
    mRefCount += 1;
    return this;
}

void IModObj::release() {
    if (--mRefCount <= 0) {
        destroy();
        delete this;
    }
}

void IModObj::destroy() {
    auto func = (void (*)(void *))_CModGetInjectedFunc(mInjectedTab, CMOD_L "destroy");
    if (func) {
        func(mInjectedObj);
    }
}

_CMOD_COLLECT_META(IModObj, intfName);
_CMOD_COLLECT_META(IModObj, retain  );
_CMOD_COLLECT_META(IModObj, release );
_CMOD_COLLECT_META(IModObj, destroy );

typedef std::map<cmod_string, void *> InjectedTab;

void *_CModGetInjectedFunc(void *injectedTab, const cmod_char *methodName) {
    if (!injectedTab || !methodName) {
        return nullptr;
    }
    
    auto tab = (InjectedTab *)injectedTab;
    auto aim = tab->find(methodName);
    if (aim == tab->end()) {
        return nullptr;
    }
    
    return aim->second;
}

//call a interface on runtime:
//

enum class PassType {
    Void  ,
    Bool  , Int   , Int64 , Float, Double,
    IntPtr, ChrPtr, ModObj,
};

union PassWord {
    bool    asBool  ;
    int     asInt   ;
    int64_t asInt64 ;
    float   asFloat ;
    double  asDouble;
    void   *asIntPtr;
};

struct PassValue {
    
    PassValue() {}
    
    PassValue(bool        v) { mType = PassType::Bool  ; mWord.asBool   = v        ; }
    PassValue(int         v) { mType = PassType::Int   ; mWord.asInt    = v        ; }
    PassValue(int64_t     v) { mType = PassType::Int64 ; mWord.asInt64  = v        ; }
    PassValue(float       v) { mType = PassType::Float ; mWord.asFloat  = v        ; }
    PassValue(double      v) { mType = PassType::Double; mWord.asDouble = v        ; }
    PassValue(const void *v) { mType = PassType::IntPtr; mWord.asIntPtr = (void *)v; }
    
    PassValue(const cmod_char *value) {
        mType   = PassType::ChrPtr;
        mString = value ? value : CMOD_L "";
    }
    
    PassValue(IModObj *value) {
        mType   = PassType::ModObj;
        mModObj = cmod_make_shared value;
    }
    
    PassType type() {
        return mType;
    }
    
    //double does not need to be reinterpreted.
    template<typename> int64_t asInt64() {
        switch (mType) {
            case PassType::Bool  : return (int64_t)mWord.asBool  ;
            case PassType::Int   : return (int64_t)mWord.asInt   ;
            case PassType::Int64 : return /* ... */mWord.asInt64 ;
            case PassType::Float : return (int64_t)mWord.asFloat ;
            case PassType::Double: return (int64_t)mWord.asDouble;
            default: return 0;
        }
    }

    template<typename R> R asBool() { return (R)(bool)asInt64<int64_t>(); }
    template<typename R> R asInt () { return (R)(int )asInt64<int64_t>(); }

    //double does not need to be reinterpreted.
    template<typename> double asDouble() {
        switch (mType) {
            case PassType::Bool  : return (double)mWord.asBool  ;
            case PassType::Int   : return (double)mWord.asInt   ;
            case PassType::Int64 : return (double)mWord.asInt64 ;
            case PassType::Float : return (double)mWord.asFloat ;
            case PassType::Double: return /* .. */mWord.asDouble;
            default: return 0.0;
        }
    }

    //float does not need to be reinterpreted.
    template<typename> float asFloat() {
        return (float)asDouble<double>();
    }

    template<typename R> R asIntPtr() {
        switch (mType) {
            case PassType::IntPtr: return (R)mWord.asIntPtr ;
            case PassType::ChrPtr: return (R)mString.c_str();
            default: return (R)0;
        }
    }

    template<typename R> R asChrPtr() {
        if (mType == PassType::ChrPtr) {
            return (R)mString.c_str();
        }
        return (R)0;
    }

    template<typename R> R asModObj() {
        if (mType == PassType::ModObj) {
            return (R)mModObj.get();
        }
        return (R)0;
    }

private:
    PassType mType = PassType::Void;
    PassWord mWord = {0};
    
    std::shared_ptr<IModObj> mModObj;
    cmod_string mString;
};

struct CModPass {
    int       argCount  = 0;
    PassValue argValues [_CModMethodMaxArgCount];
    PassValue retValue  ;
    
    CModMethod *method  = nullptr;
    IModObj    *modObj  = nullptr;
};

CModPass *CModPassCreate() {
    return new CModPass();
}

void CModPassRelease(CModPass *pass) {
    delete pass;
}

template<typename T> void PushValue(CModPass *pass, T value) {
    if (pass && pass->argCount < _CModMethodMaxArgCount) {
        pass->argValues[pass->argCount++] = value;
    }
}

void CModPassPushBool  (CModPass *pass, bool             value) { PushValue(pass, value); }
void CModPassPushInt   (CModPass *pass, int              value) { PushValue(pass, value); }
void CModPassPushInt64 (CModPass *pass, int64_t          value) { PushValue(pass, value); }
void CModPassPushFloat (CModPass *pass, float            value) { PushValue(pass, value); }
void CModPassPushDouble(CModPass *pass, double           value) { PushValue(pass, value); }
void CModPassPushIntPtr(CModPass *pass, const void      *value) { PushValue(pass, value); }
void CModPassPushChrPtr(CModPass *pass, const cmod_char *value) { PushValue(pass, value); }
void CModPassPushObj   (CModPass *pass, IModObj         *value) { PushValue(pass, value); }

static CModMethod *FindMethod(IModObj *obj, const cmod_char *methodName) {
    CModIntf *intf = CModIntfFind(obj->intfName());
    
    for (; intf; intf = CModIntfBase(intf)) {
        CModMethod *method = CModMethodFind(intf, methodName);
        if (method) {
            return method;
        }
    }
    return nullptr;
}

template<typename R, int N> struct Caller {

    template<typename... U> static R Run(CModPass *pass, U... unfold) {

        if (N == pass->method->argCount) {
            auto func = (R (*)(IModObj *, _CModVPtr *, U...))pass->method->equalFunc;
            auto vptr = &pass->method->methodVPtr;
            return func(pass->modObj, vptr, unfold...);
        }

        const cmod_char *t = pass->method->argTypes[N];
        PassValue a = pass->argValues[N];

        //NOTE:
        //function arguments only use 4 types: intptr_t, int64_t, float and double,
        //to prevent code bloat.
        
        if (!cmod_strcmp(t, _CModType<bool       >::Value)) { return Caller<R, N + 1>::Run(pass, unfold..., a.asBool  <intptr_t>()); }
        if (!cmod_strcmp(t, _CModType<int        >::Value)) { return Caller<R, N + 1>::Run(pass, unfold..., a.asInt   <intptr_t>()); }
        if (!cmod_strcmp(t, _CModType<int64_t    >::Value)) { return Caller<R, N + 1>::Run(pass, unfold..., a.asInt64 <int64_t >()); }
        if (!cmod_strcmp(t, _CModType<float      >::Value)) { return Caller<R, N + 1>::Run(pass, unfold..., a.asFloat <float   >()); }
        if (!cmod_strcmp(t, _CModType<double     >::Value)) { return Caller<R, N + 1>::Run(pass, unfold..., a.asDouble<double  >()); }
        if (!cmod_strcmp(t, _CModType<void      *>::Value)) { return Caller<R, N + 1>::Run(pass, unfold..., a.asIntPtr<intptr_t>()); }
        if (!cmod_strcmp(t, _CModType<cmod_char *>::Value)) { return Caller<R, N + 1>::Run(pass, unfold..., a.asChrPtr<intptr_t>()); }

        if (a.type() == PassType::ModObj && a.asModObj<IModObj *>()) {
            CModIntf *src = CModIntfFind(a.asModObj<IModObj *>()->intfName());
            CModIntf *dst = CModIntfFind(t);
            if (CModIntfAssignable(src, dst)) {
                return Caller<R, N + 1>::Run(pass, unfold..., a.asModObj<intptr_t>());
            }
        }
        
        return Caller<R, N + 1>::Run(pass, unfold..., (intptr_t)0);
    }
};

template<typename R> struct Caller<R, _CModMethodMaxArgCount> {

    template<typename... U> static R Run(CModPass *pass, U... unfold) {
        auto func = (R (*)(IModObj *, _CModVPtr *, U...))pass->method->equalFunc;
        auto vptr = &pass->method->methodVPtr;
        return func(pass->modObj, vptr, unfold...);
    }
};

bool CModPassCall(CModPass *pass, IModObj *obj, const cmod_char *methodName) {
    if (!pass || !obj || !methodName) {
        return false;
    }
    //a pass can only be used once.
    if (pass->method) {
        return false;
    }
    
    CModMethod *method = FindMethod(obj, methodName);
    if (!method) {
        return false;
    }
    
    pass->method = method;
    pass->modObj = obj;
    
    const cmod_char *t = method->retType;
    PassValue r;
    
    //NOTE:
    //return value only use 5 types: void, intptr_t, int64_t, float and double,
    //to prevent code bloat.
    
    if /**/ (!cmod_strcmp(t, _CModType<void       >::Value)) { /* ... ... ... */Caller<void    , 0>::Run(pass); }
    else if (!cmod_strcmp(t, _CModType<bool       >::Value)) { r = (bool       )Caller<intptr_t, 0>::Run(pass); }
    else if (!cmod_strcmp(t, _CModType<int        >::Value)) { r = (int        )Caller<intptr_t, 0>::Run(pass); }
    else if (!cmod_strcmp(t, _CModType<int64_t    >::Value)) { r = (int64_t    )Caller<int64_t , 0>::Run(pass); }
    else if (!cmod_strcmp(t, _CModType<float      >::Value)) { r = (float      )Caller<float   , 0>::Run(pass); }
    else if (!cmod_strcmp(t, _CModType<double     >::Value)) { r = (double     )Caller<double  , 0>::Run(pass); }
    else if (!cmod_strcmp(t, _CModType<void      *>::Value)) { r = (void      *)Caller<intptr_t, 0>::Run(pass); }
    else if (!cmod_strcmp(t, _CModType<cmod_char *>::Value)) { r = (cmod_char *)Caller<intptr_t, 0>::Run(pass); }
    else {
        //must be a MObject.
        r = (IModObj *)Caller<intptr_t, 0>::Run(pass);
        if (method->retRetain) {
            r.asModObj<IModObj *>()->release();
        }
    }
    pass->retValue = r;
    
    return true;
}

bool       CModPassRetBool  (CModPass *p) { return p ? p->retValue.asBool  <bool       >() : false  ; }
int        CModPassRetInt   (CModPass *p) { return p ? p->retValue.asInt   <int        >() : 0      ; }
int64_t    CModPassRetInt64 (CModPass *p) { return p ? p->retValue.asInt64 <int64_t    >() : 0      ; }
float      CModPassRetFloat (CModPass *p) { return p ? p->retValue.asFloat <float      >() : 0.0f   ; }
double     CModPassRetDouble(CModPass *p) { return p ? p->retValue.asDouble<double     >() : 0.0    ; }
void      *CModPassRetIntPtr(CModPass *p) { return p ? p->retValue.asIntPtr<void      *>() : nullptr; }
cmod_char *CModPassRetChrPtr(CModPass *p) { return p ? p->retValue.asChrPtr<cmod_char *>() : nullptr; }

IModObj *CModPassRetainRet(CModPass *pass) {
    if (!pass) {
        return nullptr;
    }
    
    auto obj = pass->retValue.asModObj<IModObj *>();
    if (!obj) {
        return nullptr;
    }
    
    return obj->retain();
}

//generate a class on runtime:
//

struct CModCls {
    CModIntf *intf = nullptr;
    InjectedTab injectedTab;
};

CModCls *CModClsImplement(const cmod_char *intfName) {
    CModIntf *intf = GetIntf(intfName, false);
    if (!intf) {
        return nullptr;
    }

    //intentional leak.
    auto cls = new CModCls();
    if (cls) {
        cls->intf = intf;
    }
    return cls;
}

void CModDefineMethod(CModCls *cls, const cmod_char *methodName, void *func) {
    if (!cls || !methodName || !func) {
        return;
    }

    //NOTE: even if the method is not declared in the interface, it also can be injected.
    //this way can override the parent method.
    cls->injectedTab[methodName] = func;
}

IModObj *CModCreateObj(CModCls *cls, void *injectedObj) {
    if (!cls) {
        return nullptr;
    }

    struct Shell : IModObj {
        void setCore(void *tab, void *obj) {
            mInjectedTab = tab;
            mInjectedObj = obj;
        };
    };

    auto shell = (Shell *)cls->intf->createShellObj();
    shell->setCore(&cls->injectedTab, injectedObj);
    return shell;
}
