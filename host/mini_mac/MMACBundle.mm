#import "MMACBundle.h"

void MMACBundle::install() {
    auto obj = MMACBundle::create();
    setInstance(obj);
}

static NSString *GetAssetBundlePath() {
    NSString *mainBundle = NSBundle.mainBundle.bundlePath;
    
#if TARGET_OS_OSX
    //the asset bundle in a app bundle.
    NSString *assetBundle = [NSString stringWithFormat:@"%@/Contents/Resources/%s", mainBundle, MBundle::BundleDirectoryName];
    if ([NSFileManager.defaultManager fileExistsAtPath:assetBundle]) {
        return assetBundle;
    }
    
    //in the project directory.
    NSRange projectName = [mainBundle rangeOfString:@"fw-mini"];
    if (projectName.location != NSNotFound) {
        NSString *projectDirectory = [mainBundle substringToIndex:NSMaxRange(projectName)];
        return [NSString stringWithFormat:@"%@/appres/%s", projectDirectory, MBundle::BundleDirectoryName];
    }
    
    //in the same path as the executable.
    return [NSString stringWithFormat:@"%@/%s", mainBundle, MBundle::BundleDirectoryName];
    
#elif TARGET_OS_IOS
    return [NSString stringWithFormat:@"%@/%s", mainBundle, MBundle::BundleDirectoryName];
    
#endif
}

MVector<uint8_t>::ptr MMACBundle::loadAsset(const std::string &path) {
    if (!mBundle) {
        NSString *bundlePath = GetAssetBundlePath();
        mBundle = [NSBundle bundleWithPath:bundlePath];
    }
    
    NSString *file = [mBundle pathForResource:@(path.c_str()) ofType:nil];
    NSData   *data = [NSData dataWithContentsOfFile:file];
    
    auto content = MVector<uint8_t>::create();
    content->insert(content->end(), (uint8_t *)data.bytes, (uint8_t *)data.bytes + data.length);
    return content;
}

std::string MMACBundle::documentDirectory() {
    NSArray<NSString *> *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return paths.firstObject.UTF8String;
}

std::string MMACBundle::temporaryDirectory() {
    NSString *path = NSTemporaryDirectory();
    return path.UTF8String;
}
