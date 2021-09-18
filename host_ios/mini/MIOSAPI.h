#import <UIKit/UIKit.h>
#import "mhostapi.h"

#pragma mark - native types.

class MIOSImage : public MImage {
    
public:
    MIOSImage(UIImage *nativeImage);
    UIImage *nativeImage();
    
private:
    UIImage *mNativeImage;
};

#pragma mark - apis.

void MRegisterAPIs();
