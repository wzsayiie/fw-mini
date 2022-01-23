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

#pragma mark - color pattern.

union MIOSColorPattern {
    struct {
        uint8_t red  ;
        uint8_t green;
        uint8_t blue ;
        uint8_t alpha;
    };
    int color;
};

#pragma mark - apis.

void MRegisterAPIs();
