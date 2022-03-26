#import "MIOSDrawView.h"

@implementation MIOSDrawView

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        //by default, "UIView" will not clear the buffer before redrawing.
        self.backgroundColor = UIColor.whiteColor;
    }
    return self;
}

@end
