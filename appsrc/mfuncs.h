#pragma once

#include "mexport.h"
#include "mtypes.h"

struct MFuncInfo {
    //internal data fields:
    int Index = 0;
    std::string Name;

    //need external assignment:
    void *FuncPtr   = nullptr;
    MType RetType   = 0;
    bool  RetRetain = false;
    std::vector<MType> ArgTypes;
};

void MFuncAdd(const MFuncInfo &info);

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

extern "C" MObject *NFuncCopyCall(int index, MArray *args);

MEXPORT(MFuncsBegin   )
MEXPORT(MFuncsEnd     )
MEXPORT(MFuncIndex    )
MEXPORT(MFuncName     )
MEXPORT(MFuncRetType  )
MEXPORT(MFuncRetRetain)
MEXPORT(MFuncArgCount )
MEXPORT(MFuncArgType  )
MEXPORT(NFuncCopyCall )
