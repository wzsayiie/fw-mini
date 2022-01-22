#import <UIKit/UIKit.h>
#import "mhostapi.h"

#pragma mark - native types.

class MIOSImage : public MImage {
    
public:
    MIOSImage(UIImage *uiImage);
    UIImage *uiImage();
    
private:
    UIImage *mUIImage;
};

#pragma mark - apis.

void MRegisterAPIs();
