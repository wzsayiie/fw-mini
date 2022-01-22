#pragma once

#include "mbasics.h"

const char     *const MPrivateDirectoryU8Name  =  "mini"        M_META(MPrivateDirectoryU8Name );
const char16_t *const MPrivateDirectoryU16Name = u"mini"        M_META(MPrivateDirectoryU16Name);
const char     *const MAssetBundleU8Name       =  "mini.bundle" M_META(MAssetBundleU8Name      );
const char16_t *const MAssetBundleU16Name      = u"mini.bundle" M_META(MAssetBundleU16Name     );

m_class(MImage, "Img");

typedef void     (*_MApi_PrintMessage     )(MString *text);
typedef MData   *(*_MApi_CopyBundleAsset  )(MString *path);
typedef MImage  *(*_MApi_CreateImage      )(MData   *data);
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
M_FUNC_HOST void _MSetApi_CopyDocumentPath (_MApi_CopyDocumentPath  func);
M_FUNC_HOST void _MSetApi_CopyCachePath    (_MApi_CopyCachePath     func);
M_FUNC_HOST void _MSetApi_CopyTemporaryPath(_MApi_CopyTemporaryPath func);
M_FUNC_HOST void _MSetApi_MakeDirectory    (_MApi_MakeDirectory     func);
M_FUNC_HOST void _MSetApi_CopyPathSubItems (_MApi_CopyPathSubItems  func);
M_FUNC_HOST void _MSetApi_RemovePath       (_MApi_RemovePath        func);
M_FUNC_HOST void _MSetApi_PathExists       (_MApi_PathExists        func);
M_FUNC_HOST void _MSetApi_DirectoryExists  (_MApi_DirectoryExists   func);
M_FUNC_HOST void _MSetApi_FileExists       (_MApi_FileExists        func);

M_FUNC_EXPORT void     MPrintMessage     (MString *text) M_META(MPrintMessage     , "args:text");
M_FUNC_EXPORT MData   *MCopyBundleAsset  (MString *path) M_META(MCopyBundleAsset  , "args:path");
M_FUNC_EXPORT MImage  *MCreateImage      (MData   *data) M_META(MCreateImage      , "args:data");
M_FUNC_EXPORT MString *MCopyDocumentPath ()              M_META(MCopyDocumentPath );
M_FUNC_EXPORT MString *MCopyCachePath    ()              M_META(MCopyCachePath    );
M_FUNC_EXPORT MString *MCopyTemporaryPath()              M_META(MCopyTemporaryPath);
M_FUNC_EXPORT bool     MMakeDirectory    (MString *path) M_META(MMakeDirectory    , "args:path");
M_FUNC_EXPORT MArray  *MCopyPathSubItems (MString *path) M_META(MCopyPathSubItems , "args:path");
M_FUNC_EXPORT void     MRemovePath       (MString *path) M_META(MRemovePath       , "args:path");
M_FUNC_EXPORT bool     MPathExists       (MString *path) M_META(MPathExists       , "args:path");
M_FUNC_EXPORT bool     MDirectoryExists  (MString *path) M_META(MDirectoryExists  , "args:path");
M_FUNC_EXPORT bool     MFileExists       (MString *path) M_META(MFileExists       , "args:path");
