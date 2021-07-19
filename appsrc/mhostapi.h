#pragma once

#include "mtypes.h"

static const char     *const _MPrivateDirectoryU8Name  =  "mini";
static const char16_t *const _MPrivateDirectoryU16Name = u"mini";
static const char     *const _MAssetBundleU8Name       =  "mini.bundle";
static const char16_t *const _MAssetBundleU16Name      = u"mini.bundle";

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

void     MPrintMessage     (MString *text);
MData   *MCopyBundleAsset  (MString *path);
MImage  *MCreateImage      (MData   *data);
MString *MCopyDocumentPath ();
MString *MCopyCachePath    ();
MString *MCopyTemporaryPath();
bool     MMakeDirectory    (MString *path);
MArray  *MCopyPathSubItems (MString *path);
void     MRemovePath       (MString *path);
bool     MPathExists       (MString *path);
bool     MDirectoryExists  (MString *path);
bool     MFileExists       (MString *path);

MFUNC_EXPORT(MPrintMessage     )
MFUNC_EXPORT(MCopyBundleAsset  )
MFUNC_EXPORT(MCreateImage      )
MFUNC_EXPORT(MCopyDocumentPath )
MFUNC_EXPORT(MCopyCachePath    )
MFUNC_EXPORT(MCopyTemporaryPath)
MFUNC_EXPORT(MMakeDirectory    )
MFUNC_EXPORT(MCopyPathSubItems )
MFUNC_EXPORT(MRemovePath       )
MFUNC_EXPORT(MPathExists       )
MFUNC_EXPORT(MDirectoryExists  )
MFUNC_EXPORT(MFileExists       )
