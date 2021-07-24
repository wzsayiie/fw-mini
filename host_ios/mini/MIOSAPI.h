#import <UIKit/UIKit.h>
#import "mtypes.h"

#pragma mark - native types.

class MImageLoad : public _MNative {
    
public:
    MImageLoad(UIImage *nativeImage);
    UIImage *nativeImage();
    
private:
    UIImage *mNativeImage;
};

#pragma mark - apis.

void MRegisterAPIs();
