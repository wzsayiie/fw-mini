#pragma once

#include "mbasics.h"

const char     *const MPrivateDirectoryU8Name  =  "mini"        M_META(MPrivateDirectoryU8Name );
const char16_t *const MPrivateDirectoryU16Name = u"mini"        M_META(MPrivateDirectoryU16Name);
const char     *const MAssetBundleU8Name       =  "mini.bundle" M_META(MAssetBundleU8Name      );
const char16_t *const MAssetBundleU16Name      = u"mini.bundle" M_META(MAssetBundleU16Name     );

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

M_FUNC_EXPORT void     MPrintMessage     (MString *text) M_META(MPrintMessage     );
M_FUNC_EXPORT MData   *MCopyBundleAsset  (MString *path) M_META(MCopyBundleAsset  );
M_FUNC_EXPORT MImage  *MCreateImage      (MData   *data) M_META(MCreateImage      );
M_FUNC_EXPORT MString *MCopyDocumentPath ()              M_META(MCopyDocumentPath );
M_FUNC_EXPORT MString *MCopyCachePath    ()              M_META(MCopyCachePath    );
M_FUNC_EXPORT MString *MCopyTemporaryPath()              M_META(MCopyTemporaryPath);
M_FUNC_EXPORT bool     MMakeDirectory    (MString *path) M_META(MMakeDirectory    );
M_FUNC_EXPORT MArray  *MCopyPathSubItems (MString *path) M_META(MCopyPathSubItems );
M_FUNC_EXPORT void     MRemovePath       (MString *path) M_META(MRemovePath       );
M_FUNC_EXPORT bool     MPathExists       (MString *path) M_META(MPathExists       );
M_FUNC_EXPORT bool     MDirectoryExists  (MString *path) M_META(MDirectoryExists  );
M_FUNC_EXPORT bool     MFileExists       (MString *path) M_META(MFileExists       );
