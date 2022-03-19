#include "mhostapi.h"

static bool Enable_PrintMessage(MString *text) {
    return true;
}

static bool Enable_CopyBundleAsset(MString *path) {
    return path != nullptr;
}

static bool Enable_CreateImage(MData *data) {
    return MDataSize(data) > 0;
}

static bool Enable_CreateBitmapImage(MData *data, int w, int h) {
    if (w > 0 && h > 0) {
        return w * h == MDataSize(data) * 4;
    }
    return false;
}

static bool Enable_CopyImageBitmap (MImage *img) { return img != nullptr; }
static bool Enable_ImagePixelWidth (MImage *img) { return img != nullptr; }
static bool Enable_ImagePixelHeight(MImage *img) { return img != nullptr; }

static bool Enable_CopyDocumentPath () { return true; }
static bool Enable_CopyCachePath    () { return true; }
static bool Enable_CopyTemporaryPath() { return true; }

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
/**/        }                                       \
/**/        if (!Enable_##NAME(__VA_ARGS__)) {      \
/**/            return (RET)0;                      \
/**/        }                                       \
/**/        return s##NAME(__VA_ARGS__);            \
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
