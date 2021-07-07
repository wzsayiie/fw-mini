#include "mhostapis.h"
#include <map>
#include "mdebug.h"

static std::map<MApi, MApiCopyFunc> *GetFuncs() {
    static std::map<MApi, MApiCopyFunc> *funcs = nullptr;
    if (!funcs) {
        funcs = new std::map<MApi, MApiCopyFunc>;
    }
    return funcs;
}

void _MApiSetFunc(MApi api, MApiCopyFunc func) {
    if (!func) {
        D("ERROR: register a null api");
        return;
    }

    std::map<MApi, MApiCopyFunc> *funcs = GetFuncs();
    if (funcs->find(api) != funcs->end()) {
        D("ERROR: api '%s' already exists.", (const char *)&api);
        return;
    }

    (*funcs)[api] = func;
}

MObject *MApiCopyCall(MApi api, MObject *a, MObject *b) {
    std::map<MApi, MApiCopyFunc> *funcs = GetFuncs();
    auto pair = funcs->find(api);
    if (pair == funcs->end()) {
        D("ERROR: not found api %d", api);
        return nullptr;
    }

    MApiCopyFunc func = pair->second;
    return func(a, b);
}
