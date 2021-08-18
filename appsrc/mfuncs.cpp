#include "mfuncs.h"

typedef std::map<std::string, _MFuncMeta> FuncMap;

static FuncMap *GetFuncMap() {
    static FuncMap *map = nullptr;
    if (!map) {
        map = new FuncMap;
    }
    return map;
}

static FuncMap::iterator *GetIterator() {
    static FuncMap::iterator *iterator = nullptr;
    if (!iterator) {
        iterator = new FuncMap::iterator;
    }
    return iterator;
}

#define sFuncMap  (*GetFuncMap ())
#define sIterator (*GetIterator())

void _MFuncSetMeta(const char *name, const _MFuncMeta& meta) {
    if (name) {
        sFuncMap[name] = meta;
    }
}

void MFuncsIteratorBegin() {
    sIterator = sFuncMap.begin();
}

bool MFuncsIteratorValid() {
    return sIterator != sFuncMap.end();
}

void MFuncsIteratorNext() {
    ++sIterator;
}

bool MFuncSelect(const char *name) {
    if (name) {
        sIterator = sFuncMap.find(name);
    } else {
        sIterator = sFuncMap.end();
    }

    return sIterator != sFuncMap.end();
}

const char *MFuncName() {
    return sIterator->first.c_str();
}

MType MFuncRetType  () { return sIterator->second.retType  ; }
bool  MFuncRetRetain() { return sIterator->second.retRetain; }
int   MFuncArgCount () { return sIterator->second.argCount ; }
MType MFuncArgType0 () { return sIterator->second.argType0 ; }
MType MFuncArgType1 () { return sIterator->second.argType1 ; }
MType MFuncArgType2 () { return sIterator->second.argType2 ; }
MType MFuncArgType3 () { return sIterator->second.argType3 ; }

MObject *MFuncCallCopy(const char *name, MObject *a0, MObject *a1, MObject *a2) {
    return nullptr;
}
