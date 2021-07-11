#import "MViewController.h"
#import "MAPIs.h"
#import "MView.h"
#import "mapp.h"
#import "mhostui.h"

@implementation MViewController

- (void)loadView {
    //NOTE: don't call [super loadView], that will try to unarchive a nib.
    self.view = [[MView alloc] initWithFrame:NSMakeRect(0, 0, 360, 640)];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    MRegisterAPIs();
    
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
    if (event.modifierFlags & NSEventModifierFlagDeviceIndependentFlagsMask) {
        return;
    }
    
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
