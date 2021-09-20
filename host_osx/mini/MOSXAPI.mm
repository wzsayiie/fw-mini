#import "MOSXAPI.h"

#pragma mark - native types.

MOSXImage::MOSXImage(NSImage *nativeImage) {
    mNativeImage = nativeImage;
}

NSImage *MOSXImage::nativeImage() {
    return mNativeImage;
}

#pragma mark - apis.

static void PrintMessage(MString *text) {
    NSLog(@"%s", MStringU8Chars(text));
}

static MData *CopyBundleAsset(MString *path) {
    static NSBundle *bundle = nil;
    if (!bundle) {
        NSString *mainPath = NSBundle.mainBundle.bundlePath;
        
        NSString *pathInBundle  = [NSString stringWithFormat:@"%@/Contents/Resources/%s", mainPath, MAssetBundleU8Name];
        NSString *pathInFlat    = [NSString stringWithFormat:@"%@/%s", mainPath, MAssetBundleU8Name];
        NSString *pathInProject = nil;
        
        NSRange projectName = [mainPath rangeOfString:@"fw-mini"];
        if (projectName.location != NSNotFound) {
            NSString *base = [mainPath substringToIndex:NSMaxRange(projectName)];
            pathInProject = [NSString stringWithFormat:@"%@/appres/%s", base, MAssetBundleU8Name];
        }
        
        NSFileManager *manager = NSFileManager.defaultManager;
        if /**/ ([manager fileExistsAtPath:pathInBundle ]) { bundle = [NSBundle bundleWithPath:pathInBundle ]; }
        else if ([manager fileExistsAtPath:pathInFlat   ]) { bundle = [NSBundle bundleWithPath:pathInFlat   ]; }
        else if ([manager fileExistsAtPath:pathInProject]) { bundle = [NSBundle bundleWithPath:pathInProject]; }
    }
    
    NSString *assetPath = [bundle pathForResource:@(MStringU8Chars(path)) ofType:nil];
    NSData   *assetData = [NSData dataWithContentsOfFile:assetPath];
    
    return MDataCreate((const uint8_t *)assetData.bytes, (int)assetData.length);
}

static MImage *CreateImage(MData *data) {
    NSData  *imageData   = [NSData dataWithBytes:MDataBytes(data) length:MDataSize(data)];
    NSImage *imageObject = [[NSImage alloc] initWithData:imageData];
    
    if (imageObject) {
        return new MOSXImage(imageObject);
    }
    return nullptr;
}

static MString *CopyDocumentPath() {
    NSArray<NSString *> *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return MStringCreateU8(paths.firstObject.UTF8String);
}

static MString *CopyCachePath() {
    NSArray<NSString *> *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    return MStringCreateU8(paths.firstObject.UTF8String);
}

static MString *CopyTemporaryPath() {
    NSString *path = NSTemporaryDirectory();
    return MStringCreateU8(path.UTF8String);
}

static bool MakeDirectory(MString *path) {
    NSFileManager *manager = NSFileManager.defaultManager;
    return [manager createDirectoryAtPath:@(MStringU8Chars(path)) withIntermediateDirectories:YES attributes:nil error:NULL];
}

MArray *CopyPathSubItems(MString *path) {
    NSFileManager *manager = NSFileManager.defaultManager;
    NSArray<NSString *> *names = [manager contentsOfDirectoryAtPath:@(MStringU8Chars(path)) error:nil];

    //NOTE: need to sort.
    names = [names sortedArrayUsingComparator:^NSComparisonResult(NSString *a, NSString *b) {
        return [a compare:b];
    }];

    MArray *items = MArrayCreate();
    for (NSString *name in names) {
        MStringRef item = m_auto_release MStringCreateU8(name.UTF8String);
        MArrayAppend(items, item.get());
    }
    return items;
}

static void RemovePath(MString *path) {
    NSFileManager *manager = NSFileManager.defaultManager;
    [manager removeItemAtPath:@(MStringU8Chars(path)) error:NULL];
}

static bool PathExists(MString *path) {
    NSFileManager *manager = NSFileManager.defaultManager;
    return [manager fileExistsAtPath:@(MStringU8Chars(path)) isDirectory:NULL];
}

static bool DirectoryExists(MString *path) {
    NSFileManager *manager = NSFileManager.defaultManager;
    
    BOOL isDirectory = NO;
    BOOL exist = [manager fileExistsAtPath:@(MStringU8Chars(path)) isDirectory:&isDirectory];
    
    return exist && isDirectory;
}

static bool FileExists(MString *path) {
    NSFileManager *manager = NSFileManager.defaultManager;
    
    BOOL isDirectory = NO;
    BOOL exist = [manager fileExistsAtPath:@(MStringU8Chars(path)) isDirectory:&isDirectory];
    
    return exist && !isDirectory;
}

void MRegisterAPIs() {
    _MSetApiPrintMessage     (PrintMessage     );
    _MSetApiCopyBundleAsset  (CopyBundleAsset  );
    _MSetApiCreateImage      (CreateImage      );
    _MSetApiCopyDocumentPath (CopyDocumentPath );
    _MSetApiCopyCachePath    (CopyCachePath    );
    _MSetApiCopyTemporaryPath(CopyTemporaryPath);
    _MSetApiMakeDirectory    (MakeDirectory    );
    _MSetApiCopyPathSubItems (CopyPathSubItems );
    _MSetApiRemovePath       (RemovePath       );
    _MSetApiPathExists       (PathExists       );
    _MSetApiDirectoryExists  (DirectoryExists  );
    _MSetApiFileExists       (FileExists       );
}
