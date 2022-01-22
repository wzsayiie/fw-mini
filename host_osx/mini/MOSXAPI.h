#import <Cocoa/Cocoa.h>
#import "mhostapi.h"

#pragma mark - native types.

class MOSXImage : public MImage {
    
public:
    MOSXImage(NSImage *nsImage);
    NSImage *nsImage();
    
private:
    NSImage *mNSImage;
};

#pragma mark - apis.

void MRegisterAPIs();
