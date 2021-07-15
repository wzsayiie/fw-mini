#pragma once

#include "mexport.h"
#include "mtypes.h"

typedef void     (*_MApiPrintMessage     )(MString *text);
typedef MData   *(*_MApiCopyBundleAsset  )(MString *path);
typedef MImage  *(*_MApiCreateImage      )(MData   *data);
typedef MString *(*_MApiCopyDocumentPath )();
typedef MString *(*_MApiCopyCachePath    )();
typedef MString *(*_MApiCopyTemporaryPath)();
typedef bool     (*_MApiMakeDirectory    )(MString *path);
typedef void     (*_MApiRemovePath       )(MString *path);
typedef bool     (*_MApiPathExists       )(MString *path);
typedef bool     (*_MApiDirectoryExists  )(MString *path);
typedef bool     (*_MApiFileExists       )(MString *path);

extern "C" void _MSetApiPrintMessage     (_MApiPrintMessage      func);
extern "C" void _MSetApiCopyBundleAsset  (_MApiCopyBundleAsset   func);
extern "C" void _MSetApiCreateImage      (_MApiCreateImage       func);
extern "C" void _MSetApiCopyDocumentPath (_MApiCopyDocumentPath  func);
extern "C" void _MSetApiCopyCachePath    (_MApiCopyCachePath     func);
extern "C" void _MSetApiCopyTemporaryPath(_MApiCopyTemporaryPath func);
extern "C" void _MSetApiMakeDirectory    (_MApiMakeDirectory     func);
extern "C" void _MSetApiRemovePath       (_MApiRemovePath        func);
extern "C" void _MSetApiPathExists       (_MApiPathExists        func);
extern "C" void _MSetApiDirectoryExists  (_MApiDirectoryExists   func);
extern "C" void _MSetApiFileExists       (_MApiFileExists        func);

void     MPrintMessage     (MString *text);
MData   *MCopyBundleAsset  (MString *path);
MImage  *MCreateImage      (MData   *data);
MString *MCopyDocumentPath ();
MString *MCopyCachePath    ();
MString *MCopyTemporaryPath();
bool     MMakeDirectory    (MString *path);
void     MRemovePath       (MString *path);
bool     MPathExists       (MString *path);
bool     MDirectoryExists  (MString *path);
bool     MFileExists       (MString *path);

MEXPORT(MPrintMessage     )
MEXPORT(MCopyBundleAsset  )
MEXPORT(MCreateImage      )
MEXPORT(MCopyDocumentPath )
MEXPORT(MCopyCachePath    )
MEXPORT(MCopyTemporaryPath)
MEXPORT(MMakeDirectory    )
MEXPORT(MRemovePath       )
MEXPORT(MPathExists       )
MEXPORT(MDirectoryExists  )
MEXPORT(MFileExists       )
