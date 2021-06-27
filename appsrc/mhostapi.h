#pragma once

#include "mexport.h"
#include "mtypes.h"

typedef int MApi;

const MApi MApi_Print             = 101;    //void (string text);
const MApi MApi_RunThread         = 201;    //void (lambda proc);
const MApi MApi_ThreaSleep        = 202;    //void (float seconds);
const MApi MApi_CopyResource      = 301;    //data (string path);
const MApi MApi_CopyDocumentPath  = 401;    //string ();
const MApi MApi_CopyCachePath     = 402;    //string ();
const MApi MApi_CopyTemporaryPath = 403;    //string ();
const MApi MApi_MakeDirectory     = 404;    //bool (string path);
const MApi MApi_RemovePath        = 405;    //void (string path);
const MApi MApi_PathExist         = 406;    //bool (string path);
const MApi MApi_DirectoryExist    = 407;    //bool (string path);
const MApi MApi_FileExist         = 408;    //bool (string path);

typedef MObject *(*MApiCopyFunc)(MArray *params);

//the host need registering apis by this function.
extern "C" void _MApiSetFunc(MApi api, MApiCopyFunc func);

//to call host api by this function.
MObject *MApiCopyCall(MApi api, MArray *params);

MEXPORT(MApiCopyCall);
