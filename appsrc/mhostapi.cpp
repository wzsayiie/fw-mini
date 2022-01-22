#include "mhostapi.h"

template<typename F> class ApiObject;

template<typename R, typename... A> class ApiObject<R (A...)> {
    
public:
    void operator=(R (*func)(A...)) {
        mFunc = func;
    }
    
    R operator()(const char *name, A... params) {
        if (mFunc) {
            return mFunc(params...);
        } else {
            D("ERROR: no api '%s'", name);
            return (R) 0;
        }
    }
    
private:
    R (*mFunc)(A...);
};

static ApiObject<void     (MString *)> sPrintMessage     ;
static ApiObject<MData   *(MString *)> sCopyBundleAsset  ;
static ApiObject<MImage  *(MData   *)> sCreateImage      ;
static ApiObject<MString *()         > sCopyDocumentPath ;
static ApiObject<MString *()         > sCopyCachePath    ;
static ApiObject<MString *()         > sCopyTemporaryPath;
static ApiObject<bool     (MString *)> sMakeDirectory    ;
static ApiObject<MArray  *(MString *)> sCopyPathSubItems ;
static ApiObject<void     (MString *)> sRemovePath       ;
static ApiObject<bool     (MString *)> sPathExists       ;
static ApiObject<bool     (MString *)> sDirectoryExists  ;
static ApiObject<bool     (MString *)> sFileExists       ;

void _MSetApi_PrintMessage     (_MApi_PrintMessage      func) { sPrintMessage      = func; }
void _MSetApi_CopyBundleAsset  (_MApi_CopyBundleAsset   func) { sCopyBundleAsset   = func; }
void _MSetApi_CreateImage      (_MApi_CreateImage       func) { sCreateImage       = func; }
void _MSetApi_CopyDocumentPath (_MApi_CopyDocumentPath  func) { sCopyDocumentPath  = func; }
void _MSetApi_CopyCachePath    (_MApi_CopyCachePath     func) { sCopyCachePath     = func; }
void _MSetApi_CopyTemporaryPath(_MApi_CopyTemporaryPath func) { sCopyTemporaryPath = func; }
void _MSetApi_MakeDirectory    (_MApi_MakeDirectory     func) { sMakeDirectory     = func; }
void _MSetApi_CopyPathSubItems (_MApi_CopyPathSubItems  func) { sCopyPathSubItems  = func; }
void _MSetApi_RemovePath       (_MApi_RemovePath        func) { sRemovePath        = func; }
void _MSetApi_PathExists       (_MApi_PathExists        func) { sPathExists        = func; }
void _MSetApi_DirectoryExists  (_MApi_DirectoryExists   func) { sDirectoryExists   = func; }
void _MSetApi_FileExists       (_MApi_FileExists        func) { sFileExists        = func; }

void     MPrintMessage     (MString *text) { return sPrintMessage     (__func__, text); }
MData   *MCopyBundleAsset  (MString *path) { return sCopyBundleAsset  (__func__, path); }
MImage  *MCreateImage      (MData   *data) { return sCreateImage      (__func__, data); }
MString *MCopyDocumentPath ()              { return sCopyDocumentPath (__func__);       }
MString *MCopyCachePath    ()              { return sCopyCachePath    (__func__);       }
MString *MCopyTemporaryPath()              { return sCopyTemporaryPath(__func__);       }
bool     MMakeDirectory    (MString *path) { return sMakeDirectory    (__func__, path); }
MArray  *MCopyPathSubItems (MString *path) { return sCopyPathSubItems (__func__, path); }
void     MRemovePath       (MString *path) { return sRemovePath       (__func__, path); }
bool     MPathExists       (MString *path) { return sPathExists       (__func__, path); }
bool     MDirectoryExists  (MString *path) { return sDirectoryExists  (__func__, path); }
bool     MFileExists       (MString *path) { return sFileExists       (__func__, path); }
