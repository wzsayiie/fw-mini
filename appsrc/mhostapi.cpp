#include "mhostapi.h"

#define IMPL(RET, NAME, ARGS, ...)                  \
/**/    static _MApi_##NAME s##NAME = nullptr;      \
/**/                                                \
/**/    void _MSetApi_##NAME(_MApi_##NAME func) {   \
/**/        s##NAME = func;                         \
/**/    }                                           \
/**/                                                \
/**/    RET M##NAME ARGS {                          \
/**/        if (!s##NAME) {                         \
/**/            D("ERROR: no api '%s'", "M"#NAME);  \
/**/            return (RET)0;                      \
/**/        } else {                                \
/**/            return s##NAME(__VA_ARGS__);        \
/**/        }                                       \
/**/    }

IMPL(MImage  *, CreateBitmapImage, (MData   *data , int w, int h), data, w, h);
IMPL(void     , PrintMessage     , (MString *text), text);
IMPL(MData   *, CopyBundleAsset  , (MString *path), path);
IMPL(MImage  *, CreateImage      , (MData   *data), data);
IMPL(MData   *, CopyImageBitmap  , (MImage  *img ), img );
IMPL(int      , ImagePixelWidth  , (MImage  *img ), img );
IMPL(int      , ImagePixelHeight , (MImage  *img ), img );
IMPL(MString *, CopyDocumentPath , ());
IMPL(MString *, CopyCachePath    , ());
IMPL(MString *, CopyTemporaryPath, ());
IMPL(bool     , MakeDirectory    , (MString *path), path);
IMPL(MArray  *, CopyPathSubItems , (MString *path), path);
IMPL(void     , RemovePath       , (MString *path), path);
IMPL(bool     , PathExists       , (MString *path), path);
IMPL(bool     , DirectoryExists  , (MString *path), path);
IMPL(bool     , FileExists       , (MString *path), path);
