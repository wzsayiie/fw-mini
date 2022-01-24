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

static NSBundle *GetResBundlePath() {
    NSString *mainPath = NSBundle.mainBundle.bundlePath;
    
    NSString *pathInApp = [NSString stringWithFormat:@"%@/Contents/Resources/%s", mainPath, MAssetBundleU8Name];
    if ([NSFileManager.defaultManager fileExistsAtPath:pathInApp]) {
        return [NSBundle bundleWithPath:pathInApp];
    }
    
    //if the executable is in the project, use the resources from the project.
    NSRange projectName = [mainPath rangeOfString:@"fw-mini"];
    if (projectName.location != NSNotFound) {
        NSString *projectPath   = [mainPath substringToIndex:NSMaxRange(projectName)];
        NSString *pathInProject = [NSString stringWithFormat:@"%@/appres/%s", projectPath, MAssetBundleU8Name];
        return [NSBundle bundleWithPath:pathInProject];
    }
    
    //if the executable is not in an app bundle, "mainPath" is the directory where the executable is located.
    NSString *pathWithExe = [NSString stringWithFormat:@"%@/%s", mainPath, MAssetBundleU8Name];
    if ([NSFileManager.defaultManager fileExistsAtPath:pathWithExe]) {
        return [NSBundle bundleWithPath:pathWithExe];
    }
    
    return nil;
}

static MData *CopyBundleAsset(MString *path) {
    static NSBundle *bundle = GetResBundlePath();
    
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
}
