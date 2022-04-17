#import "MMACBundle.h"

void MMACBundle::install() {
    auto obj = MMACBundle::create();
    setSharedObject(obj);
}

MVector<uint8_t>::ptr MMACBundle::loadAsset(const std::string &path) {
    if (!mBundle) {
        NSString *appBundlePath = NSBundle.mainBundle.bundlePath;
        
    #if TARGET_OS_OSX
        NSString *resBundlePath = [NSString stringWithFormat:@"%@/Contents/Resources/%s", appBundlePath, MBundle::BundleDirectoryName];
    #elif TARGET_OS_IOS
        NSString *resBundlePath = [NSString stringWithFormat:@"%@/%s", appBundlePath, MBundle::BundleDirectoryName];
    #endif
        
        mBundle = [NSBundle bundleWithPath:resBundlePath];
    }
    
    NSString *file = [mBundle pathForResource:@(path.c_str()) ofType:nil];
    NSData   *data = [NSData dataWithContentsOfFile:file];
    
    auto content = MVector<uint8_t>::create();
    content->vector.insert(content->vector.end(), (uint8_t *)data.bytes, (uint8_t *)data.bytes + data.length);
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
