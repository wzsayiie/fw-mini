#pragma once

#include "mtypes.h"

struct MFuncInfo {
    //internal data fields:
    int index = 0;
    std::string name;

    //need external assignment:
    void *funcPtr   = nullptr;
    MType retType   = 0;
    bool  retRetain = false;
    std::vector<MType> argTypes;
};

void _MFuncAdd(const MFuncInfo &info);

extern "C" int MFuncsBegin();
extern "C" int MFuncsEnd  ();

//if the function not be found, return -1.
extern "C" int MFuncIndex(const char *name);

//NOTE!
//the parameter "index" must be valid:

extern "C" const char *MFuncName(int index);

extern "C" MType MFuncRetType  (int index);
extern "C" bool  MFuncRetRetain(int index);
extern "C" int   MFuncArgCount (int index);
extern "C" MType MFuncArgType  (int index, int argIndex);

extern "C" MObject *NFuncCopyCall (int index, MArray *params);
extern "C" void     NFuncCallVoid (int index, MArray *params);
extern "C" bool     NFuncCallBool (int index, MArray *params);
extern "C" int      NFuncCallInt  (int index, MArray *params);
extern "C" float    NFuncCallFloat(int index, MArray *params);
