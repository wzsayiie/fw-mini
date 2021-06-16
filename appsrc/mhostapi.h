#pragma once

#include "mexport.h"
#include "mtypes.h"

typedef int MApi;

const MApi MApi_Print           = 101;   //void (string text);
const MApi MApi_PostOnMain      = 201;   //void (lambda proc);
const MApi MApi_RunThread       = 202;   //void (lambda proc);
const MApi MApi_ThreaSleep      = 203;   //void (float seconds);
const MApi MApi_LoadResource    = 301;   //data (string path);
const MApi MApi_DocumentFolder  = 401;   //string ();
const MApi MApi_CacheFolder     = 402;   //string ();
const MApi MApi_TemporaryFolder = 403;   //string ();
const MApi MApi_MakeFolder      = 404;   //bool (string path);
const MApi MApi_RemovePath      = 405;   //void (string path);
const MApi MApi_PathExist       = 406;   //bool (string path);
const MApi MApi_FolderExist     = 407;   //bool (string path);
const MApi MApi_FileExist       = 408;   //bool (string path);

typedef MObject *(*MApiFunc)(MArray *params);

//the host need registering apis by this function.
extern "C" void MApiSetFunc(MApi api, MApiFunc func);

//to call host api by this function.
MObject *MApiCopyCall(MApi api, MArray *params);

MEXPORT(MApiCopyCall);
