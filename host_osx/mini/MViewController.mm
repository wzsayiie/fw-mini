#import "MViewController.h"
#import "mapp.h"
#import "mhostui.h"

@implementation MView

- (void)drawRect:(NSRect)dirtyRect {
    _MWindowOnDraw();
    
    CGContextRef context = NSGraphicsContext.currentContext.CGContext;
    float height = self.frame.size.height;

    int triangleCount = _MWindowTriangleCount();
    for (int index = 0; index < triangleCount; ++index) {
        [self context:context drawTriangle:index viewHeight:height];
    }

    int labelCount = _MWindowLabelCount();
    for (int index = 0; index < labelCount; ++index) {
        [self context:context drawLabel:index viewHeight:height];
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

- (void)context:(CGContextRef)context drawTriangle:(int)index viewHeight:(float)height {
    MColor rgba = _MWindowTriangleColor(index);
    [self context:context setFillStrokeRGBA:rgba];
    CGContextSetLineWidth(context, 1);
    
    float x0 = _MWindowTriangleVertexX(index, 0);
    float y0 = _MWindowTriangleVertexY(index, 0);
    float x1 = _MWindowTriangleVertexX(index, 1);
    float y1 = _MWindowTriangleVertexY(index, 1);
    float x2 = _MWindowTriangleVertexX(index, 2);
    float y2 = _MWindowTriangleVertexY(index, 2);
    
    CGContextMoveToPoint   (context, x0, height - y0);
    CGContextAddLineToPoint(context, x1, height - y1);
    CGContextAddLineToPoint(context, x2, height - y2);
    
    CGContextDrawPath(context, kCGPathFillStroke);
}

- (void)context:(CGContextRef)context drawLabel:(int)index viewHeight:(float)height {
}

@end

@implementation MViewController

- (void)loadView {
    //NOTE: don't call [super loadView], that will try to unarchive a nib.
    self.view = [[MView alloc] initWithFrame:NSMakeRect(0, 0, 360, 640)];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    //application events.
    _MAppLaunch();
    [NSTimer scheduledTimerWithTimeInterval:_MAppUpdateInterval repeats:YES block:^(NSTimer *timer) {
        _MAppUpdate();
    }];
    
    //window events:
    NSSize size = self.view.frame.size;
    _MWindowOnResize(size.width, size.height);
    _MWindowOnLoad();
    
    __weak NSView *weakView = self.view;
    [NSTimer scheduledTimerWithTimeInterval:_MWindowDrawInterval repeats:YES block:^(NSTimer *timer) {
        [weakView setNeedsDisplay:YES];
    }];
    
    NSNotificationCenter *center = NSNotificationCenter.defaultCenter;
    [center addObserver:self selector:@selector(windowDidBecomeKey)   name:NSWindowDidBecomeKeyNotification   object:nil];
    [center addObserver:self selector:@selector(windowDidMiniaturize) name:NSWindowDidMiniaturizeNotification object:nil];
    [center addObserver:self selector:@selector(windowDidResize)      name:NSWindowDidResizeNotification      object:nil];
    
    __weak MViewController *weakController = self;
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown handler:^NSEvent *(NSEvent *event) {
        [weakController keyboardKeyDown:event];
        return nil;
    }];
}

- (void)windowDidBecomeKey {
    _MWindowOnShow();
}

- (void)windowDidMiniaturize {
    _MWindowOnHide();
}

- (void)windowDidResize {
    NSSize size = self.view.frame.size;
    _MWindowOnResize(size.width, size.height);
}

- (void)mouseEvent:(NSEvent *)event func:(void (*)(float, float))func {
    NSPoint point = [event locationInWindow];
    func(point.x, self.view.frame.size.height - point.y);
}

- (void)mouseDown   :(NSEvent *)event { [self mouseEvent:event func:_MWindowOnTouchBegin]; }
- (void)mouseDragged:(NSEvent *)event { [self mouseEvent:event func:_MWindowOnTouchMove ]; }
- (void)mouseUp     :(NSEvent *)event { [self mouseEvent:event func:_MWindowOnTouchEnd  ]; }

- (void)keyboardKeyDown:(NSEvent *)event {
    switch (event.keyCode) {
        case  51: _MWindowOnKeyDown(MKey_Back ); break;
        case  36: _MWindowOnKeyDown(MKey_Enter); break;
        case  49: _MWindowOnKeyDown(MKey_Space); break;
        case 123: _MWindowOnKeyDown(MKey_Left ); break;
        case 126: _MWindowOnKeyDown(MKey_Up   ); break;
        case 124: _MWindowOnKeyDown(MKey_Right); break;
        case 125: _MWindowOnKeyDown(MKey_Down ); break;
        case   0: _MWindowOnKeyDown(MKey_A    ); break;
        case  13: _MWindowOnKeyDown(MKey_W    ); break;
        case   2: _MWindowOnKeyDown(MKey_D    ); break;
        case   1: _MWindowOnKeyDown(MKey_S    ); break;

        default:;
    }
}

@end
