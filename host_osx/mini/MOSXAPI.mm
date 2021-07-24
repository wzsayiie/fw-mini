#import "MOSXAPI.h"
#import "mhostapi.h"

#pragma mark - native types.

MImageLoad::MImageLoad(NSImage *nativeImage) {
    mNativeImage = nativeImage;
}

NSImage *MImageLoad::nativeImage() {
    return mNativeImage;
}

#pragma mark - apis.

static void PrintMessage(MString *text) {
    NSLog(@"%s", MStringU8Chars(text));
}

static MData *CopyBundleAsset(MString *path) {
    NSBundle *bundle = nil;
    if (!bundle) {
        NSString *appMainPath = NSBundle.mainBundle.bundlePath;
        NSString *bundlePath  = [NSString stringWithFormat:@"%@/Contents/Resources/%s", appMainPath, _MAssetBundleU8Name];
        bundle = [NSBundle bundleWithPath:bundlePath];
    }
    
    NSString *assetPath = [bundle pathForResource:@(MStringU8Chars(path)) ofType:nil];
    NSData   *assetData = [NSData dataWithContentsOfFile:assetPath];
    
    return MDataCreate((const uint8_t *)assetData.bytes, (int)assetData.length);
}

static MImage *CreateImage(MData *data) {
    NSData  *imageData   = [NSData dataWithBytes:MDataBytes(data) length:MDataSize(data)];
    NSImage *imageObject = [[NSImage alloc] initWithData:imageData];
    
    if (imageObject) {
        return MImageCreate(new MImageLoad(imageObject));
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
