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

M_FUNC_HOST void _MSetApiPrintMessage     (_MApiPrintMessage      func);
M_FUNC_HOST void _MSetApiCopyBundleAsset  (_MApiCopyBundleAsset   func);
M_FUNC_HOST void _MSetApiCreateImage      (_MApiCreateImage       func);
M_FUNC_HOST void _MSetApiCopyDocumentPath (_MApiCopyDocumentPath  func);
M_FUNC_HOST void _MSetApiCopyCachePath    (_MApiCopyCachePath     func);
M_FUNC_HOST void _MSetApiCopyTemporaryPath(_MApiCopyTemporaryPath func);
M_FUNC_HOST void _MSetApiMakeDirectory    (_MApiMakeDirectory     func);
M_FUNC_HOST void _MSetApiCopyPathSubItems (_MApiCopyPathSubItems  func);
M_FUNC_HOST void _MSetApiRemovePath       (_MApiRemovePath        func);
M_FUNC_HOST void _MSetApiPathExists       (_MApiPathExists        func);
M_FUNC_HOST void _MSetApiDirectoryExists  (_MApiDirectoryExists   func);
M_FUNC_HOST void _MSetApiFileExists       (_MApiFileExists        func);

M_FUNC_EXPORT void     MPrintMessage     (MString *text) MFUNC_META(MPrintMessage     );
M_FUNC_EXPORT MData   *MCopyBundleAsset  (MString *path) MFUNC_META(MCopyBundleAsset  );
M_FUNC_EXPORT MImage  *MCreateImage      (MData   *data) MFUNC_META(MCreateImage      );
M_FUNC_EXPORT MString *MCopyDocumentPath ()              MFUNC_META(MCopyDocumentPath );
M_FUNC_EXPORT MString *MCopyCachePath    ()              MFUNC_META(MCopyCachePath    );
M_FUNC_EXPORT MString *MCopyTemporaryPath()              MFUNC_META(MCopyTemporaryPath);
M_FUNC_EXPORT bool     MMakeDirectory    (MString *path) MFUNC_META(MMakeDirectory    );
M_FUNC_EXPORT MArray  *MCopyPathSubItems (MString *path) MFUNC_META(MCopyPathSubItems );
M_FUNC_EXPORT void     MRemovePath       (MString *path) MFUNC_META(MRemovePath       );
M_FUNC_EXPORT bool     MPathExists       (MString *path) MFUNC_META(MPathExists       );
M_FUNC_EXPORT bool     MDirectoryExists  (MString *path) MFUNC_META(MDirectoryExists  );
M_FUNC_EXPORT bool     MFileExists       (MString *path) MFUNC_META(MFileExists       );
