#import <UIKit/UIKit.h>
#import "mtypes.h"

#pragma mark - native types.

class MImageLoad : public MSpecial {
    
public:
    MImageLoad(UIImage *nativeImage);
    UIImage *nativeImage();
    
private:
    UIImage *mNativeImage;
};

#pragma mark - apis.

void MRegisterAPIs();
