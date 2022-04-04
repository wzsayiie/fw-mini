#import "MOSXBundle.h"

void MOSXBundle::install() {
    auto obj = MOSXBundle::create();
    setSharedObject(obj);
}

MVector<uint8_t>::ptr MOSXBundle::loadAsset(const std::string &path) {
    if (!mBundle) {
        NSString *mainPath   = NSBundle.mainBundle.bundlePath;
        NSString *bundlePath = [NSString stringWithFormat:@"%@/Contents/Resources/%s", mainPath, MBundle::BundleDirectoryName];
        
        mBundle = [NSBundle bundleWithPath:bundlePath];
    }
    
    NSString *file = [mBundle pathForResource:@(path.c_str()) ofType:nil];
    NSData   *data = [NSData dataWithContentsOfFile:file];
    
    auto content = MVector<uint8_t>::create();
    content->vector.insert(content->vector.end(), (uint8_t *)data.bytes, (uint8_t *)data.bytes + data.length);
    return content;
}

std::string MOSXBundle::documentDirectory() {
    NSArray<NSString *> *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return paths.firstObject.UTF8String;
}

std::string MOSXBundle::temporaryDirectory() {
    NSString *path = NSTemporaryDirectory();
    return path.UTF8String;
}
