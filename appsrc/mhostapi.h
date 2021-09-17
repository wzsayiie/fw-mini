#pragma once

#include "mbasics.h"

static const char     *const _MPrivateDirectoryU8Name  =  "mini";
static const char16_t *const _MPrivateDirectoryU16Name = u"mini";
static const char     *const _MAssetBundleU8Name       =  "mini.bundle";
static const char16_t *const _MAssetBundleU16Name      = u"mini.bundle";

m_class(MImage, "Img") {};

typedef void     (*_MApiPrintMessage     )(MString *text);
typedef MData   *(*_MApiCopyBundleAsset  )(MString *path);
typedef MImage  *(*_MApiCreateImage      )(MData   *data);
typedef MString *(*_MApiCopyDocumentPath )();
typedef MString *(*_MApiCopyCachePath    )();
typedef MString *(*_MApiCopyTemporaryPath)();
typedef bool     (*_MApiMakeDirectory    )(MString *path);
typedef MArray  *(*_MApiCopyPathSubItems )(MString *path);
typedef void     (*_MApiRemovePath       )(MString *path);
typedef bool     (*_MApiPathExists       )(MString *path);
typedef bool     (*_MApiDirectoryExists  )(MString *path);
typedef bool     (*_MApiFileExists       )(MString *path);

MFUNC_HOST void _MSetApiPrintMessage     (_MApiPrintMessage      func);
MFUNC_HOST void _MSetApiCopyBundleAsset  (_MApiCopyBundleAsset   func);
MFUNC_HOST void _MSetApiCreateImage      (_MApiCreateImage       func);
MFUNC_HOST void _MSetApiCopyDocumentPath (_MApiCopyDocumentPath  func);
MFUNC_HOST void _MSetApiCopyCachePath    (_MApiCopyCachePath     func);
MFUNC_HOST void _MSetApiCopyTemporaryPath(_MApiCopyTemporaryPath func);
MFUNC_HOST void _MSetApiMakeDirectory    (_MApiMakeDirectory     func);
MFUNC_HOST void _MSetApiCopyPathSubItems (_MApiCopyPathSubItems  func);
MFUNC_HOST void _MSetApiRemovePath       (_MApiRemovePath        func);
MFUNC_HOST void _MSetApiPathExists       (_MApiPathExists        func);
MFUNC_HOST void _MSetApiDirectoryExists  (_MApiDirectoryExists   func);
MFUNC_HOST void _MSetApiFileExists       (_MApiFileExists        func);

MFUNC_EXPORT void     MPrintMessage     (MString *text) MFUNC_META(MPrintMessage     );
MFUNC_EXPORT MData   *MCopyBundleAsset  (MString *path) MFUNC_META(MCopyBundleAsset  );
MFUNC_EXPORT MImage  *MCreateImage      (MData   *data) MFUNC_META(MCreateImage      );
MFUNC_EXPORT MString *MCopyDocumentPath ()              MFUNC_META(MCopyDocumentPath );
MFUNC_EXPORT MString *MCopyCachePath    ()              MFUNC_META(MCopyCachePath    );
MFUNC_EXPORT MString *MCopyTemporaryPath()              MFUNC_META(MCopyTemporaryPath);
MFUNC_EXPORT bool     MMakeDirectory    (MString *path) MFUNC_META(MMakeDirectory    );
MFUNC_EXPORT MArray  *MCopyPathSubItems (MString *path) MFUNC_META(MCopyPathSubItems );
MFUNC_EXPORT void     MRemovePath       (MString *path) MFUNC_META(MRemovePath       );
MFUNC_EXPORT bool     MPathExists       (MString *path) MFUNC_META(MPathExists       );
MFUNC_EXPORT bool     MDirectoryExists  (MString *path) MFUNC_META(MDirectoryExists  );
MFUNC_EXPORT bool     MFileExists       (MString *path) MFUNC_META(MFileExists       );
