#pragma once

#include "mtypes.h"

typedef int MApi;

const MApi MApi_Print             = MEnumId("Prt"); //void (string text);
const MApi MApi_CopyResource      = MEnumId("CpR"); //data (string path);
const MApi MApi_CreateImage       = MEnumId("CrI"); //image (data content);
const MApi MApi_CopyDocumentPath  = MEnumId("DcP"); //string ();
const MApi MApi_CopyCachePath     = MEnumId("CcP"); //string ();
const MApi MApi_CopyTemporaryPath = MEnumId("TmP"); //string ();
const MApi MApi_MakeDirectory     = MEnumId("MkD"); //bool (string path);
const MApi MApi_RemovePath        = MEnumId("RmD"); //void (string path);
const MApi MApi_PathExists        = MEnumId("PEx"); //bool (string path);
const MApi MApi_DirectoryExists   = MEnumId("DEx"); //bool (string path);
const MApi MApi_FileExists        = MEnumId("FEx"); //bool (string path);

typedef MObject *(*MApiCopyFunc)(MObject *a, MObject *b);

//the host need registering apis by this function.
extern "C" void _MApiSetFunc(MApi api, MApiCopyFunc func);

//to call host api by this function.
MObject *MApiCopyCall(MApi api, MObject *a, MObject *b);

MEXPORT(MApiCopyCall);
