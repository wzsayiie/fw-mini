#import "MViewController.h"
#import "mapp.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(CGRect)rect {
    _MWindowOnDraw();
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        [self makeContext:context drawTriangleWithIndex:index];
    }
    
    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        [self makeContext:context drawLabelWithIndex:index];
    }
}

- (void)makeContext:(CGContextRef)context setFillStrokeColorWithRGBA:(MColor)rgba {
    auto color = (MColorPattern *)&rgba;
    
    float red   = color->red   / 255.f;
    float green = color->green / 255.f;
    float blue  = color->blue  / 255.f;
    float alpha = color->alpha / 255.f;
    
    CGContextSetRGBFillColor  (context, red, green, blue, alpha);
    CGContextSetRGBStrokeColor(context, red, green, blue, alpha);
}

- (void)makeContext:(CGContextRef)context drawTriangleWithIndex:(int)index {
    MColor rgba = _MWindowTriangleColor(index);
    [self makeContext:context setFillStrokeColorWithRGBA:rgba];
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

- (void)makeContext:(CGContextRef)context drawLabelWithIndex:(int)index {
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
    _MWindowOnLoad();
    
    __weak UIView *weakView = self.view;
    [NSTimer scheduledTimerWithTimeInterval:_MWindowDrawInterval repeats:YES block:^(NSTimer *timer) {
        [weakView setNeedsDisplay];
    }];
    
    NSNotificationCenter *center = NSNotificationCenter.defaultCenter;
    [center addObserver:self
               selector:@selector(handleApplicationWillEnterForeground)
                   name:UIApplicationWillEnterForegroundNotification
                 object:nil];
    [center addObserver:self
               selector:@selector(handleApplicationWillResignActive)
                   name:UIApplicationWillResignActiveNotification
                 object:nil];
}

- (void)viewWillAppear:(BOOL)animated {
    CGSize size = self.view.frame.size;
    _MWindowOnResize(size.width, size.height);
    _MWindowOnShow();
}

- (void)handleApplicationWillEnterForeground {
    _MWindowOnShow();
}

- (void)handleApplicationWillResignActive {
    _MWindowOnHide();
}

- (void)viewWillTransitionToSize:(CGSize)size
       withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    _MWindowOnResize(size.width, size.height);
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint point = [touches.anyObject locationInView:self.view];
    _MWindowOnTouchBegin(point.x, point.y);
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint point = [touches.anyObject locationInView:self.view];
    _MWindowOnTouchMove(point.x, point.y);
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint point = [touches.anyObject locationInView:self.view];
    _MWindowOnTouchEnd(point.x, point.y);
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    CGPoint point = [touches.anyObject locationInView:self.view];
    _MWindowOnTouchEnd(point.x, point.y);
}

@end
