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

#pragma mark - color pattern.

union MOSXColorPattern {
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
