#import "MOSXViewController.h"
#import "MMACBundle.h"
#import "MMACDrawView.h"
#import "MMACImageFactory.h"
#import <Carbon/Carbon.h>

const CGFloat ViewContentWidth  = 360;
const CGFloat ViewContentHeight = 640;
const CGFloat TextFieldWidth    = 300;
const CGFloat TextFieldHeight   =  20;
const CGFloat TextFieldBottom   =  20;

@implementation MOSXViewController

- (NSTextField *)textField {
    if (!_textField) {
        _textField = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, TextFieldWidth, TextFieldHeight)];
        _textField.delegate = self;
        _textField.hidden = YES;
        
        [self.view addSubview:_textField];
    }
    return _textField;
}

- (MApp *)app {
    if (!_app) {
        _app = MApp::instance();
    }
    return _app;
}

- (MWindow *)window {
    if (!_window) {
        _window = MWindow::mainWindow();
    }
    return _window;
}

- (void)loadView {
    //NOTE: don't call [super loadView], that will try to unarchive a nib.
    self.view = [[MMACDrawView alloc] initWithFrame:NSMakeRect(0, 0, ViewContentWidth, ViewContentHeight)];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    MMACBundle::install();
    MMACImageFactory::install();
    
    //application events:
    self.app->launch();
    
    [NSTimer scheduledTimerWithTimeInterval:MApp::UpdateEverySeconds repeats:YES block:^(NSTimer *timer) {
        self.app->update();
    }];
    
    //window events:
    NSSize size = self.view.frame.size;
    self.window->resizePixel(size.width, size.height);
    self.window->load();
    
    [self updateTextFieldFrame];
    [NSTimer scheduledTimerWithTimeInterval:MWindow::UpdateEverySeconds repeats:YES block:^(NSTimer *timer) {
        [self.view setNeedsDisplay:YES];
        [self updateTextFieldState];
    }];
    
    //notifications listening:
    NSNotificationCenter *center = NSNotificationCenter.defaultCenter;
    [center addObserver:self selector:@selector(windowDidBecomeKey)   name:NSWindowDidBecomeKeyNotification   object:nil];
    [center addObserver:self selector:@selector(windowDidMiniaturize) name:NSWindowDidMiniaturizeNotification object:nil];
    [center addObserver:self selector:@selector(windowDidResize)      name:NSWindowDidResizeNotification      object:nil];
    
    //NOTE: the monitor will intercept ALL key down events.
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown handler:^NSEvent *(NSEvent *event) {
        return [self keyboardKeyDown:event];
    }];
}

- (void)windowDidBecomeKey {
    self.window->show();
}

- (void)windowDidMiniaturize {
    self.window->hide();
}

- (void)windowDidResize {
    [self updateTextFieldFrame];
    
    NSSize size = self.view.frame.size;
    self.window->resizePixel(size.width, size.height);
}

- (void)handleMouseMove:(NSEvent *)event func:(void (MWindow::*)(float x, float y))func {
    NSRect  bounds = self.view.bounds;
    NSPoint fromBL = [event locationInWindow];
    NSPoint fromUL = NSMakePoint(fromBL.x, bounds.size.height - fromBL.y);
    
    //on windows, if the mouse is moved out of the window, the mouse move message will not be received.
    //here is to ensure consistency across platforms.
    if (NSPointInRect(fromUL, bounds)) {
        self.window->mouseMovePixel(fromUL.x, fromUL.y);
    }
    
    if (func) {
        (self.window->*func)(fromUL.x, fromUL.y);
    }
}

- (void)mouseDown   :(NSEvent *)event { [self handleMouseMove:event func:&MWindow::touchBeginPixel]; }
- (void)mouseDragged:(NSEvent *)event { [self handleMouseMove:event func:&MWindow::touchMovePixel ]; }
- (void)mouseUp     :(NSEvent *)event { [self handleMouseMove:event func:&MWindow::touchEndPixel  ]; }
- (void)mouseMoved  :(NSEvent *)event { [self handleMouseMove:event func:nullptr                  ]; }

- (void)scrollWheel:(NSEvent *)event {
    //the value representation of the mouse wheel change is different on different platforms.
    //here is experience value.
    self.window->mouseWheel(event.deltaY * 20);
}

- (NSEvent *)keyboardKeyDown:(NSEvent *)event {
    //do not process keys with modifier keys.
    if (event.modifierFlags & NSEventModifierFlagDeviceIndependentFlagsMask) {
        return event;
    }
    
    if (!self.textField.hidden) {
        if (event.keyCode == kVK_Return) {
            [self makeControlTextEnter];
            return nil;
        } else {
            //the event needs to continue processing.
            return event;
        }
    }
    
    switch (event.keyCode) {
        case kVK_Delete    : self.window->key(MKey::Back ); break;
        case kVK_Return    : self.window->key(MKey::Enter); break;
        case kVK_Space     : self.window->key(MKey::Space); break;
            
        case kVK_LeftArrow : self.window->key(MKey::Left ); break;
        case kVK_UpArrow   : self.window->key(MKey::Up   ); break;
        case kVK_RightArrow: self.window->key(MKey::Right); break;
        case kVK_DownArrow : self.window->key(MKey::Down ); break;
        
        case kVK_ANSI_A    : self.window->key(MKey::A    ); break;
        case kVK_ANSI_D    : self.window->key(MKey::D    ); break;
        case kVK_ANSI_S    : self.window->key(MKey::S    ); break;
        case kVK_ANSI_W    : self.window->key(MKey::W    ); break;
    }
    return nil;
}

- (void)updateTextFieldFrame {
    NSSize space = self.view.frame.size;
    NSRect frame = self.textField.frame;
    
    frame.origin.x = (space.width - frame.size.width) / 2;
    frame.origin.y = TextFieldBottom;
    
    self.textField.frame = frame;
}

- (void)updateTextFieldState {
    if (!self.window->checkWritingUpdated()) {
        return;
    }
    
    if (self.window->checkWritingEnabled()) {
        [self.textField setStringValue:@(self.window->checkWritingRawText().c_str())];
        [self.textField setHidden:NO];
        [self.textField becomeFirstResponder];
        
    } else {
        [self.textField resignFirstResponder];
        [self.textField setHidden:YES];
    }
}

- (void)makeControlTextEnter {
    std::string text(self.textField.stringValue.UTF8String);
    self.window->write(text, true);
}

- (void)controlTextDidChange:(NSNotification *)notification {
    std::string text(self.textField.stringValue.UTF8String);
    self.window->write(text, false);
}

@end
