#import "MOSXFrameSaver.h"

static NSString *MOSXWindowOriginX = @"MOSXWindowOriginX";
static NSString *MOSXWindowOriginY = @"MOSXWindowOriginY";
static NSString *MOSXContentWidth  = @"MOSXContentWidth" ;
static NSString *MOSXContentHeight = @"MOSXContentHeight";

@implementation MOSXFrameSaver

+ (instancetype)sharedSaver {
    static id object = [[MOSXFrameSaver alloc] init];
    return object;
}

- (instancetype)init {
    if (self = [super init]) {
        
        NSUserDefaults *defaults = NSUserDefaults.standardUserDefaults;
        CGFloat x = [defaults doubleForKey:MOSXWindowOriginX];
        CGFloat y = [defaults doubleForKey:MOSXWindowOriginY];
        CGFloat w = [defaults doubleForKey:MOSXContentWidth ];
        CGFloat h = [defaults doubleForKey:MOSXContentHeight];
        
        //default location.
        if (w <= 0 || h <= 0) {
            w = 900;
            h = 600;
            x = (NSScreen.mainScreen.frame.size.width  - w) / 2;
            y = (NSScreen.mainScreen.frame.size.height - h) / 2;
        }
        
        self.windowOrigin = NSMakePoint(x, y);
        self.contentSize  = NSMakeSize (w, h);
    }
    return self;
}

- (void)save {
    NSUserDefaults *defaults = NSUserDefaults.standardUserDefaults;
    [defaults setDouble:self.windowOrigin.x     forKey:MOSXWindowOriginX];
    [defaults setDouble:self.windowOrigin.y     forKey:MOSXWindowOriginY];
    [defaults setDouble:self.contentSize.width  forKey:MOSXContentWidth ];
    [defaults setDouble:self.contentSize.height forKey:MOSXContentHeight];
}

@end
