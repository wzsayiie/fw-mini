#import <Cocoa/Cocoa.h>
#import "mhostapi.h"

#pragma mark - native types.

class MOSXImage : public MImage {
    
public:
    MOSXImage(NSImage *nativeImage);
    NSImage *nativeImage();
    
private:
    NSImage *mNativeImage;
};

#pragma mark - apis.

void MRegisterAPIs();
