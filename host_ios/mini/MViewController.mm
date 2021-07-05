#import "MViewController.h"
#import "MView.h"
#import "mapp.h"
#import "mhostui.h"

@implementation MViewController

- (void)loadView {
    self.view = [[MView alloc] init];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    
    //application events.
    _MAppLaunch();
    [NSTimer scheduledTimerWithTimeInterval:_MAppUpdateInterval repeats:YES block:^(NSTimer *timer) {
        _MAppUpdate();
    }];
    
    //window events:
    CGSize size = UIScreen.mainScreen.bounds.size;
    _MWindowOnResize(size.width, size.height);
    _MWindowOnLoad();
    
    __weak UIView *weakView = self.view;
    [NSTimer scheduledTimerWithTimeInterval:_MWindowDrawInterval repeats:YES block:^(NSTimer *timer) {
        [weakView setNeedsDisplay];
    }];
    
    NSNotificationCenter *center = NSNotificationCenter.defaultCenter;
    [center addObserver:self selector:@selector(appWillEnterForeground) name:UIApplicationWillEnterForegroundNotification object:nil];
    [center addObserver:self selector:@selector(appWillResignActive)    name:UIApplicationWillResignActiveNotification    object:nil];
}

- (void)appWillEnterForeground {
    _MWindowOnShow();
}

- (void)appWillResignActive {
    _MWindowOnHide();
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    _MWindowOnResize(size.width, size.height);
}

- (void)touches:(NSSet<UITouch *> *)touches func:(void (*)(float x, float y))func {
    CGPoint point = [touches.anyObject locationInView:self.view];
    func(point.x, point.y);
}

- (void)touchesBegan    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t func:_MWindowOnTouchBegin]; }
- (void)touchesMoved    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t func:_MWindowOnTouchMove ]; }
- (void)touchesEnded    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t func:_MWindowOnTouchEnd  ]; }
- (void)touchesCancelled:(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t func:_MWindowOnTouchEnd  ]; }

@end
