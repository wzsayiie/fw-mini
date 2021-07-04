#import "MViewController.h"
#import "mapp.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(CGRect)rect {
    _MWindowOnDraw();
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        [self context:context drawTriangle:index];
    }
    
    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        [self context:context drawLabel:index];
    }
}

- (void)context:(CGContextRef)context setFillStrokeRGBA:(MColor)rgba {
    auto color = (MColorPattern *)&rgba;
    
    float red   = color->red   / 255.f;
    float green = color->green / 255.f;
    float blue  = color->blue  / 255.f;
    float alpha = color->alpha / 255.f;
    
    CGContextSetRGBFillColor  (context, red, green, blue, alpha);
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
}

- (void)context:(CGContextRef)context drawTriangle:(int)index {
    MColor rgba = _MWindowTriangleColor(index);
    [self context:context setFillStrokeRGBA:rgba];
    CGContextSetLineWidth(context, 1);
    
    float x0 = _MWindowTriangleVertexX(index, 0);
    float y0 = _MWindowTriangleVertexY(index, 0);
    float x1 = _MWindowTriangleVertexX(index, 1);
    float y1 = _MWindowTriangleVertexY(index, 1);
    float x2 = _MWindowTriangleVertexX(index, 2);
    float y2 = _MWindowTriangleVertexY(index, 2);
    
    CGContextMoveToPoint   (context, x0, y0);
    CGContextAddLineToPoint(context, x1, y1);
    CGContextAddLineToPoint(context, x2, y2);
    
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)context:(CGContextRef)context drawLabel:(int)index {
}

@end

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
