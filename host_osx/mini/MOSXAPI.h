#import <Cocoa/Cocoa.h>
#import "mtypes.h"

#pragma mark - native types.

class MImageLoad : public MSpecial {
    
public:
    MImageLoad(NSImage *nativeImage);
    NSImage *nativeImage();
    
private:
    NSImage *mNativeImage;
};

#pragma mark - apis.

void MRegisterAPIs();
