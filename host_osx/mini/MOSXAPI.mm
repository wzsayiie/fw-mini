#import "MOSXAPI.h"

#pragma mark - native types.

MOSXImage::MOSXImage(NSImage *nsImage) {
    mNSImage = nsImage;
}

NSImage *MOSXImage::nsImage() {
    return mNSImage;
}

#pragma mark - bitmap context.

static CGContextRef CreateBitmapContext(void *bytes, int width, int height) {
    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(
        /* bytesData        */ bytes,
        /* width            */ width,
        /* height           */ height,
        /* bitsPerComponent */ 8,
        /* bytesPerRow      */ width * 4,
        /* spaceColor       */ space,
        /* bitmapInfo       */ kCGBitmapByteOrderDefault | kCGImageAlphaPremultipliedLast
    );
    CGColorSpaceRelease(space);
    
    return context;
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
    
    return MDataCopy((const uint8_t *)assetData.bytes, (int)assetData.length);
}

static MImage *CreateImage(MData *data) {
    NSData  *imageData   = [NSData dataWithBytes:MDataBytes(data) length:MDataSize(data)];
    NSImage *imageObject = [[NSImage alloc] initWithData:imageData];
    
    if (imageObject) {
        return new MOSXImage(imageObject);
    }
    return nullptr;
}

static MImage *CreateBitmapImage(MData *data, int width, int height) {
    std::vector<uint8_t> bitmap(width * height * 4);
    MConvertColors(width * height, (MColorPattern *)MDataBytes(data), (MOSXColorPattern *)bitmap.data());
    
    CGContextRef context = CreateBitmapContext(bitmap.data(), width, height);
    CGImageRef   cgImage = CGBitmapContextCreateImage(context);
    
    NSImage *nsImage = [[NSImage alloc] initWithCGImage:cgImage size:NSMakeSize(width, height)];
    
    CGContextRelease(context);
    CGImageRelease(cgImage);
    
    return new MOSXImage(nsImage);
}

static MData *CopyImageBitmap(MImage *image) {
    NSImage *nsImage = ((MOSXImage *)image)->nsImage();
    auto     width   = (int)nsImage.size.width ;
    auto     height  = (int)nsImage.size.height;
    
    MData *data  = MDataCreate(width * height * 4);
    void  *bytes = (void *)MDataBytes(data);
    
    CGContextRef context = CreateBitmapContext(bytes, width, height);
    NSGraphicsContext.currentContext = [NSGraphicsContext graphicsContextWithCGContext:context flipped:NO];
    [nsImage drawInRect:NSMakeRect(0, 0, width, height)];
    NSGraphicsContext.currentContext = nil;
    CGContextRelease(context);
    
    MConvertColors(width * height, (MOSXColorPattern *)bytes, (MColorPattern *)bytes);
    
    return data;
}

static int ImagePixelWidth (MImage *image) { return ((MOSXImage *)image)->nsImage().size.width ; }
static int ImagePixelHeight(MImage *image) { return ((MOSXImage *)image)->nsImage().size.height; }

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
    _MSetApi_PrintMessage     (PrintMessage     );
    _MSetApi_CopyBundleAsset  (CopyBundleAsset  );
    _MSetApi_CreateImage      (CreateImage      );
    _MSetApi_CreateBitmapImage(CreateBitmapImage);
    _MSetApi_CopyImageBitmap  (CopyImageBitmap  );
    _MSetApi_ImagePixelWidth  (ImagePixelWidth  );
    _MSetApi_ImagePixelHeight (ImagePixelHeight );
    _MSetApi_CopyDocumentPath (CopyDocumentPath );
    _MSetApi_CopyCachePath    (CopyCachePath    );
    _MSetApi_CopyTemporaryPath(CopyTemporaryPath);
    _MSetApi_MakeDirectory    (MakeDirectory    );
    _MSetApi_CopyPathSubItems (CopyPathSubItems );
    _MSetApi_RemovePath       (RemovePath       );
    _MSetApi_PathExists       (PathExists       );
    _MSetApi_DirectoryExists  (DirectoryExists  );
    _MSetApi_FileExists       (FileExists       );
}
