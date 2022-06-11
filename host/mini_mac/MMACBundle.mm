#import "MMACBundle.h"

void MMACBundle::install() {
    auto obj = MMACBundle::create();
    setInstance(obj);
}

MVector<uint8_t>::ptr MMACBundle::onLoadAsset(const std::string &path) {
    if (!mAssetBundle) {
        std::string assetBundle = bundleDirectory();
        mAssetBundle = [NSBundle bundleWithPath:@(assetBundle.c_str())];
    }
    
    NSString *file = [mAssetBundle pathForResource:@(path.c_str()) ofType:nil];
    NSData   *data = [NSData dataWithContentsOfFile:file];
    
    auto content = MVector<uint8_t>::create();
    content->insert(content->end(), (uint8_t *)data.bytes, (uint8_t *)data.bytes + data.length);
    return content;
}

std::string MMACBundle::onGetBundleDirectory() {
    NSString *mainBundle = NSBundle.mainBundle.bundlePath;
    
#if TARGET_OS_OSX
    //the asset bundle in a app bundle.
    NSString *assetBundle = [NSString stringWithFormat:@"%@/Contents/Resources/%s", mainBundle, MBundle::BundleDirectoryName];
    if ([NSFileManager.defaultManager fileExistsAtPath:assetBundle]) {
        return assetBundle.UTF8String;
    }
    
    //in the project directory.
    NSRange projectName = [mainBundle rangeOfString:@"fw-mini"];
    if (projectName.location != NSNotFound) {
        NSString *projectDirectory = [mainBundle substringToIndex:NSMaxRange(projectName)];
        
        assetBundle = [NSString stringWithFormat:@"%@/appres/%s", projectDirectory, MBundle::BundleDirectoryName];
        return assetBundle.UTF8String;
    }
    
    //in the same path as the executable.
    assetBundle = [NSString stringWithFormat:@"%@/%s", mainBundle, MBundle::BundleDirectoryName];
    return assetBundle.UTF8String;
    
#elif TARGET_OS_IOS
    NSString *assetBundle = [NSString stringWithFormat:@"%@/%s", mainBundle, MBundle::BundleDirectoryName];
    return assetBundle.UTF8String;
    
#endif
}

std::string MMACBundle::onGetDocumentDirectory() {
    NSArray<NSString *> *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return paths.firstObject.UTF8String;
}

std::string MMACBundle::onGetTemporaryDirectory() {
    NSString *path = NSTemporaryDirectory();
    return path.UTF8String;
}
