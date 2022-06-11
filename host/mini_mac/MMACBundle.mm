#import "MMACBundle.h"

void MMACBundle::install() {
    auto obj = MMACBundle::create();
    setInstance(obj);
}

MVector<uint8_t>::ptr MMACBundle::onLoadResource(const std::string &path) {
    if (!mResBundle) {
        std::string resBundle = resBundleDirectory();
        mResBundle = [NSBundle bundleWithPath:@(resBundle.c_str())];
    }
    
    NSString *file = [mResBundle pathForResource:@(path.c_str()) ofType:nil];
    NSData   *data = [NSData dataWithContentsOfFile:file];
    
    auto content = MVector<uint8_t>::create();
    content->insert(content->end(), (uint8_t *)data.bytes, (uint8_t *)data.bytes + data.length);
    return content;
}

std::string MMACBundle::onGetResBundleDirectory() {
    NSString *mainBundle = NSBundle.mainBundle.bundlePath;
    
#if TARGET_OS_OSX
    //the resource bundle in a app bundle.
    NSString *resBundle = [NSString stringWithFormat:@"%@/Contents/Resources/%s", mainBundle, MBundle::ResBundleDirectoryName];
    if ([NSFileManager.defaultManager fileExistsAtPath:resBundle]) {
        return resBundle.UTF8String;
    }
    
    //in the project directory.
    NSRange projectName = [mainBundle rangeOfString:@"fw-mini"];
    if (projectName.location != NSNotFound) {
        NSString *projectDirectory = [mainBundle substringToIndex:NSMaxRange(projectName)];
        
        resBundle = [NSString stringWithFormat:@"%@/appres/%s", projectDirectory, MBundle::ResBundleDirectoryName];
        return resBundle.UTF8String;
    }
    
    //in the same path as the executable.
    resBundle = [NSString stringWithFormat:@"%@/%s", mainBundle, MBundle::ResBundleDirectoryName];
    return resBundle.UTF8String;
    
#elif TARGET_OS_IOS
    NSString *resBundle = [NSString stringWithFormat:@"%@/%s", mainBundle, MBundle::ResBundleDirectoryName];
    return resBundle.UTF8String;
    
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
