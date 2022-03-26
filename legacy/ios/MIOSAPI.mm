#import "MIOSAPI.h"

#pragma mark - native types.

MIOSImage::MIOSImage(UIImage *uiImage) {
    mUIImage = uiImage;
}

UIImage *MIOSImage::uiImage() {
    return mUIImage;
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
        NSString *appBundlePath = NSBundle.mainBundle.bundlePath;
        NSString *resBundlePath = [NSString stringWithFormat:@"%@/%s", appBundlePath, MAssetBundleU8Name];
        bundle = [NSBundle bundleWithPath:resBundlePath];
    }
    
    NSString *assetPath = [bundle pathForResource:@(MStringU8Chars(path)) ofType:nil];
    NSData   *assetData = [NSData dataWithContentsOfFile:assetPath];
    
    return MDataCopy((const uint8_t *)assetData.bytes, (int)assetData.length);
}

static MImage *CreateImage(MData *data) {
    NSData  *imageData   = [NSData dataWithBytes:MDataBytes(data) length:MDataSize(data)];
    UIImage *imageObject = [[UIImage alloc] initWithData:imageData];
    
    if (imageObject) {
        return new MIOSImage(imageObject);
    }
    return nullptr;
}

static MImage *CreateBitmapImage(MData *data, int width, int height) {
    std::vector<uint8_t> bitmap(width * height * 4);
    MConvertColors(width * height, (MColorPattern *)MDataBytes(data), (MIOSColorPattern *)bitmap.data());
    
    CGContextRef context = CreateBitmapContext(bitmap.data(), width, height);
    CGImageRef   cgImage = CGBitmapContextCreateImage(context);
    
    UIImage *nsImage = [UIImage imageWithCGImage:cgImage];
    
    CGContextRelease(context);
    CGImageRelease(cgImage);
    
    return new MIOSImage(nsImage);
}

static MData *CopyImageBitmap(MImage *image) {
    UIImage *uiImage = ((MIOSImage *)image)->uiImage();
    auto     width   = (int)uiImage.size.width ;
    auto     height  = (int)uiImage.size.height;
    
    MData *data  = MDataCreate(width * height * 4);
    void  *bytes = (void *)MDataBytes(data);
    
    CGContextRef context = CreateBitmapContext(bytes, width, height);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), uiImage.CGImage);
    CGContextRelease(context);
    
    MConvertColors(width * height, (MIOSColorPattern *)bytes, (MColorPattern *)bytes);
    
    return data;
}

static int ImagePixelWidth (MImage *image) { return ((MIOSImage *)image)->uiImage().size.width ; }
static int ImagePixelHeight(MImage *image) { return ((MIOSImage *)image)->uiImage().size.height; }

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
