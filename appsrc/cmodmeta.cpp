#include "cmodmeta.h"
#include <map>
#include <vector>

//interface meta store:
//

struct CModMethod : _CModMetaCommitment {
};

struct CModIntf {
    const cmod_char *intfName = nullptr;
    void *(*createNilObj)()   = nullptr;
    int vtabSize = 0;

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

    intf->createNilObj = commitment->createNilObj;
    if (intf->vtabSize < method->vtabIndex + 1) {
        intf->vtabSize = method->vtabIndex + 1;
    }

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

void *CModIntfCreateNilObj(CModIntf *intf) {
    if (intf) {
        return intf->createNilObj();
    }
    return nullptr;
}

int CModIntfVTabSize(CModIntf *intf) {
    if (intf) {
        return (int)intf->methodList.size();
    }
    return 0;
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

int CModMethodVTabIndex(CModMethod *method) {
    if (method) {
        return method->vtabIndex;
    }
    return 0;
}

void *CModMethodEqualFunc(CModMethod *method) {
    if (method) {
        return method->equalFunc;
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
    if (!mInjectedTab) {
        return;
    }

    int index = _CModGetVTabIndex(&IModObj::destroy);
    auto func = (void (*)(void *))mInjectedTab[index];
    func(mPayloadObj);
}

_CMOD_COLLECT_META(IModObj, destroy);

//generate a class on runtime:

struct CModCls {
    CModIntf *intf = nullptr;
    void *injectedTab[1] = {0};
};

CModCls *CModClsImplement(const cmod_char *intfName) {
    if (!intfName) {
        return nullptr;
    }

    CModIntf *intf = GetIntf(intfName, false);
    if (!intf) {
        return nullptr;
    }

    int tabSize = intf->vtabSize;
    int clsSize = sizeof(CModCls) + sizeof(void *) * (tabSize - 1);
    //intentional leak.
    auto cls = (CModCls *)calloc(clsSize, 1);
    if (cls) {
        cls->intf = intf;
    }
    return cls;
}

void CModDefineMethod(CModCls *cls, const cmod_char *methodName, void *func) {
    if (!cls || !methodName || !func) {
        return;
    }

    CModMethod *method = CModMethodFind(cls->intf, methodName);
    if (!method) {
        return;
    }

    int index = method->vtabIndex;
    cls->injectedTab[index] = func;
}

IModObj *CModCreateObj(CModCls *cls, void *loadObj) {
    if (!cls) {
        return nullptr;
    }

    struct Shell : IModObj {
        void setCore(void **tab, void *obj) {
            mInjectedTab = tab;
            mPayloadObj  = obj;
        };
    };

    auto shell = (Shell *)cls->intf->createNilObj();
    shell->setCore(cls->injectedTab, loadObj);
    return shell;
}
