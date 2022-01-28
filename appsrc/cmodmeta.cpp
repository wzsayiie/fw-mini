#include "cmodmeta.h"
#include <map>
#include <vector>

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
    intf->methodMap[commitment->intfName] = method;
    intf->methodList.push_back(method);
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
    if (intfName) {
        return GetIntf(intfName, false);
    }
    return nullptr;
}

const cmod_char *CModIntfName(CModIntf *intf) {
    if (intf) {
        return intf->intfName;
    }
    return nullptr;
}

void *CModIntfCreateShellObj(CModIntf *intf) {
    if (intf) {
        return intf->createShellObj();
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

const cmod_char *CModMethodName(CModMethod *method) {
    if (method) {
        return method->methodName;
    }
    return nullptr;
}

void *CModMethodEqualFunc(CModMethod *method) {
    if (method) {
        return method->equalFunc;
    }
    return nullptr;
}

_CModVPtr *CModMethodVPtr(CModMethod *method) {
    if (method) {
        return &method->methodVPtr;
    }
    return nullptr;
}

const cmod_char *CModMethodRetType(CModMethod *method) {
    if (method) {
        return method->retType;
    }
    return nullptr;
}

int CModMethodArgCount(CModMethod *method) {
    if (method) {
        return method->argCount;
    }
    return 0;
}

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

//generate a class on runtime:

struct CModCls {
    CModIntf *intf = nullptr;
    InjectedTab injectedTab;
};

CModCls *CModClsImplement(const cmod_char *intfName) {
    if (!intfName) {
        return nullptr;
    }

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
