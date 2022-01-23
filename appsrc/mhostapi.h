#pragma once

#include "mbasics.h"

//------------------------------------------------------------------------------
//configuration:

const char     *const MPrivateDirectoryU8Name  =  "mini"        M_META(MPrivateDirectoryU8Name );
const char16_t *const MPrivateDirectoryU16Name = u"mini"        M_META(MPrivateDirectoryU16Name);
const char     *const MAssetBundleU8Name       =  "mini.bundle" M_META(MAssetBundleU8Name      );
const char16_t *const MAssetBundleU16Name      = u"mini.bundle" M_META(MAssetBundleU16Name     );

//------------------------------------------------------------------------------
//color:

typedef int MColor;

union MColorPattern {
    struct {
        uint8_t alpha;
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
    };
    MColor color = 0;
};

template<typename SRC, typename DST> void MConvertColors(int count, const SRC *src, DST *dst) {
    for (int i = 0; i < count; ++i) {
        //NOTE: src and dst maybe are same.
        SRC color = src[i];

        dst[i].red   = color.red  ;
        dst[i].green = color.green;
        dst[i].blue  = color.blue ;
        dst[i].alpha = color.alpha;
    }
}

const MColor MColor_BlackColor     = 0x000000ff M_META(MColor_BlackColor    );
const MColor MColor_DarkGrayColor  = 0x404040ff M_META(MColor_DarkGrayColor );
const MColor MColor_GrayColor      = 0x808080ff M_META(MColor_GrayColor     );
const MColor MColor_LightGrayColor = 0xC0C0C0ff M_META(MColor_LightGrayColor);
const MColor MColor_WhiteColor     = 0xFFFFFFff M_META(MColor_WhiteColor    );
const MColor MColor_RedColor       = 0xC3272Bff M_META(MColor_RedColor      );
const MColor MColor_GreenColor     = 0x21A675ff M_META(MColor_GreenColor    );
const MColor MColor_BlueColor      = 0x177CB0ff M_META(MColor_BlueColor     );
const MColor MColor_CyanColor      = 0x4C8DAEff M_META(MColor_CyanColor     );
const MColor MColor_YellowColor    = 0xF2BE45ff M_META(MColor_YellowColor   );
const MColor MColor_MagentaColor   = 0xA01DAEff M_META(MColor_MagentaColor  );
const MColor MColor_OrangeColor    = 0xFFA400ff M_META(MColor_OrangeColor   );
const MColor MColor_PurpleColor    = 0x8D4BBBff M_META(MColor_PurpleColor   );
const MColor MColor_BrownColor     = 0xE29C45ff M_META(MColor_BrownColor    );
const MColor MColor_ClearColor     = 0x00000000 M_META(MColor_ClearColor    );

//------------------------------------------------------------------------------
//apis:

m_class(MImage, "Img");

typedef void     (*_MApi_PrintMessage     )(MString *text);
typedef MData   *(*_MApi_CopyBundleAsset  )(MString *path);
typedef MImage  *(*_MApi_CreateImage      )(MData   *data);
typedef MImage  *(*_MApi_CreateBitmapImage)(MData   *data, int w, int h);
typedef MData   *(*_MApi_CopyImageBitmap  )(MImage  *img );
typedef int      (*_MApi_ImagePixelWidth  )(MImage  *img );
typedef int      (*_MApi_ImagePixelHeight )(MImage  *img );
typedef MString *(*_MApi_CopyDocumentPath )();
typedef MString *(*_MApi_CopyCachePath    )();
typedef MString *(*_MApi_CopyTemporaryPath)();
typedef bool     (*_MApi_MakeDirectory    )(MString *path);
typedef MArray  *(*_MApi_CopyPathSubItems )(MString *path);
typedef void     (*_MApi_RemovePath       )(MString *path);
typedef bool     (*_MApi_PathExists       )(MString *path);
typedef bool     (*_MApi_DirectoryExists  )(MString *path);
typedef bool     (*_MApi_FileExists       )(MString *path);

M_FUNC_HOST void _MSetApi_PrintMessage     (_MApi_PrintMessage      func);
M_FUNC_HOST void _MSetApi_CopyBundleAsset  (_MApi_CopyBundleAsset   func);
M_FUNC_HOST void _MSetApi_CreateImage      (_MApi_CreateImage       func);
M_FUNC_HOST void _MSetApi_CreateBitmapImage(_MApi_CreateBitmapImage func);
M_FUNC_HOST void _MSetApi_CopyImageBitmap  (_MApi_CopyImageBitmap   func);
M_FUNC_HOST void _MSetApi_ImagePixelWidth  (_MApi_ImagePixelWidth   func);
M_FUNC_HOST void _MSetApi_ImagePixelHeight (_MApi_ImagePixelHeight  func);
M_FUNC_HOST void _MSetApi_CopyDocumentPath (_MApi_CopyDocumentPath  func);
M_FUNC_HOST void _MSetApi_CopyCachePath    (_MApi_CopyCachePath     func);
M_FUNC_HOST void _MSetApi_CopyTemporaryPath(_MApi_CopyTemporaryPath func);
M_FUNC_HOST void _MSetApi_MakeDirectory    (_MApi_MakeDirectory     func);
M_FUNC_HOST void _MSetApi_CopyPathSubItems (_MApi_CopyPathSubItems  func);
M_FUNC_HOST void _MSetApi_RemovePath       (_MApi_RemovePath        func);
M_FUNC_HOST void _MSetApi_PathExists       (_MApi_PathExists        func);
M_FUNC_HOST void _MSetApi_DirectoryExists  (_MApi_DirectoryExists   func);
M_FUNC_HOST void _MSetApi_FileExists       (_MApi_FileExists        func);

M_FUNC_EXPORT MImage *MCreateBitmapImage(MData *data, int w, int h)
    M_META(MCreateBitmapImage, "args:img,w,h");

M_FUNC_EXPORT void     MPrintMessage     (MString *text) M_META(MPrintMessage     , "args:text");
M_FUNC_EXPORT MData   *MCopyBundleAsset  (MString *path) M_META(MCopyBundleAsset  , "args:path");
M_FUNC_EXPORT MImage  *MCreateImage      (MData   *data) M_META(MCreateImage      , "args:data");
M_FUNC_EXPORT MData   *MCopyImageBitmap  (MImage  *img ) M_META(MCopyImageBitmap  , "args:img" );
M_FUNC_EXPORT int      MImagePixelWidth  (MImage  *img ) M_META(MImagePixelWidth  , "args:img" );
M_FUNC_EXPORT int      MImagePixelHeight (MImage  *img ) M_META(MImagePixelHeight , "args:img" );
M_FUNC_EXPORT MString *MCopyDocumentPath ()              M_META(MCopyDocumentPath );
M_FUNC_EXPORT MString *MCopyCachePath    ()              M_META(MCopyCachePath    );
M_FUNC_EXPORT MString *MCopyTemporaryPath()              M_META(MCopyTemporaryPath);
M_FUNC_EXPORT bool     MMakeDirectory    (MString *path) M_META(MMakeDirectory    , "args:path");
M_FUNC_EXPORT MArray  *MCopyPathSubItems (MString *path) M_META(MCopyPathSubItems , "args:path");
M_FUNC_EXPORT void     MRemovePath       (MString *path) M_META(MRemovePath       , "args:path");
M_FUNC_EXPORT bool     MPathExists       (MString *path) M_META(MPathExists       , "args:path");
M_FUNC_EXPORT bool     MDirectoryExists  (MString *path) M_META(MDirectoryExists  , "args:path");
M_FUNC_EXPORT bool     MFileExists       (MString *path) M_META(MFileExists       , "args:path");
