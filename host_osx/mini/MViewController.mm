#import "MViewController.h"
#import <Carbon/Carbon.h>
#import "MJavaScriptCore.h"
#import "MOSXAPI.h"
#import "MView.h"
#import "mhostloop.h"
#import "mhostui.h"

@implementation MViewController

- (void)loadView {
    //NOTE: don't call [super loadView], that will try to unarchive a nib.
    self.view = [[MView alloc] initWithFrame:NSMakeRect(0, 0, 360, 640)];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    MInstallJSVirtualMachine();
    MRegisterAPIs();
    
    NSSize size = self.view.frame.size;
    [self adjustedTextFieldFrameWithSuperSize:size];
    
    //application events:
    _MAppLaunch();
    
    [NSTimer scheduledTimerWithTimeInterval:_MAppUpdateInterval repeats:YES block:^(NSTimer *timer) {
        [self updateTextFieldState];
        _MAppUpdate();
    }];
    
    //window events:
    _MWindowOnResize(size.width, size.height);
    _MWindowOnLoad();
    
    [NSTimer scheduledTimerWithTimeInterval:_MWindowDrawInterval repeats:YES block:^(NSTimer *timer) {
        [self.view setNeedsDisplay:YES];
    }];
    
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
    _MWindowOnShow();
}

- (void)windowDidMiniaturize {
    _MWindowOnHide();
}

- (void)windowDidResize {
    NSSize size = self.view.frame.size;
    [self adjustedTextFieldFrameWithSuperSize:size];
    _MWindowOnResize(size.width, size.height);
}

- (void)handleMouseMove:(NSEvent *)event func:(void (*)(float, float))func {
    NSRect  bounds = self.view.bounds;
    NSPoint fromBL = [event locationInWindow];
    NSPoint fromUL = NSMakePoint(fromBL.x, bounds.size.height - fromBL.y);
    
    if (NSPointInRect(fromUL, bounds)) {
        _MWindowOnMouseMove(fromUL.x, fromUL.y);
    }
    if (func) {
        func(fromUL.x, fromUL.y);
    }
}

- (void)mouseDown   :(NSEvent *)event { [self handleMouseMove:event func:_MWindowOnTouchBegin]; }
- (void)mouseDragged:(NSEvent *)event { [self handleMouseMove:event func:_MWindowOnTouchMove ]; }
- (void)mouseUp     :(NSEvent *)event { [self handleMouseMove:event func:_MWindowOnTouchEnd  ]; }
- (void)mouseMoved  :(NSEvent *)event { [self handleMouseMove:event func:NULL                ]; }

- (NSTextField *)textField {
    if (!_textField) {
        _textField = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 20)];
        _textField.delegate = self;
        _textField.hidden = YES;
        
        [self.view addSubview:_textField];
    }
    return _textField;
}

- (void)adjustedTextFieldFrameWithSuperSize:(NSSize)superSize {
    NSRect frame = self.textField.frame;
    
    frame.origin.x = (superSize.width - frame.size.width) / 2;
    frame.origin.y = 20;
    
    self.textField.frame = frame;
}

- (void)updateTextFieldState {
    if (!_MWindowTextBoxUpdated()) {
        return;
    }
    
    //IMPORTANT: reset the flag;
    MWindowSetTextBoxUpdated(false);
    
    if (_MWindowTextBoxEnabled()) {
        self.textFieldEditing = YES;
        
        MString  *raw  = _MWindowTextBoxRawString();
        NSString *text = [NSString stringWithUTF8String:MStringU8Chars(raw)];
        
        [self.textField setStringValue:text];
        [self.textField setHidden:NO];
        [self.textField becomeFirstResponder];
        
    } else {
        self.textFieldEditing = NO;
        
        [self.textField resignFirstResponder];
        [self.textField setHidden:YES];
    }
}

- (void)makeTextBoxEventWithEnter:(bool)enter {
    NSString  *value  = self.textField.stringValue;
    MStringRef string = m_auto_release MStringCreateU8(value.UTF8String);
    
    _MWindowOnTextBox(string.get(), enter);
}

- (void)controlTextDidChange:(NSNotification *)notification {
    [self makeTextBoxEventWithEnter:false];
}

- (NSEvent *)keyboardKeyDown:(NSEvent *)event {
    //do not process keys with modifier keys.
    if (event.modifierFlags & NSEventModifierFlagDeviceIndependentFlagsMask) {
        return event;
    }
    
    if (self.textFieldEditing) {
        if (event.keyCode == 36 /* enter */) {
            [self makeTextBoxEventWithEnter:true];
            return nil;
        } else {
            //pass the event to the textfield.
            return event;
        }
    }
    
    switch (event.keyCode) {
        
        case kVK_Delete    : _MWindowOnKeyDown(MKey_Back ); break;
        case kVK_Return    : _MWindowOnKeyDown(MKey_Enter); break;
        case kVK_Space     : _MWindowOnKeyDown(MKey_Space); break;
        case kVK_LeftArrow : _MWindowOnKeyDown(MKey_Left ); break;
        case kVK_UpArrow   : _MWindowOnKeyDown(MKey_Up   ); break;
        case kVK_RightArrow: _MWindowOnKeyDown(MKey_Right); break;
        case kVK_DownArrow : _MWindowOnKeyDown(MKey_Down ); break;
        
        case kVK_ANSI_A: _MWindowOnKeyDown(MKey_A); break;
        case kVK_ANSI_B: _MWindowOnKeyDown(MKey_B); break;
        case kVK_ANSI_C: _MWindowOnKeyDown(MKey_C); break;
        case kVK_ANSI_D: _MWindowOnKeyDown(MKey_D); break;
        case kVK_ANSI_E: _MWindowOnKeyDown(MKey_E); break;
        case kVK_ANSI_F: _MWindowOnKeyDown(MKey_F); break;
        case kVK_ANSI_G: _MWindowOnKeyDown(MKey_G); break;
        case kVK_ANSI_H: _MWindowOnKeyDown(MKey_H); break;
        case kVK_ANSI_I: _MWindowOnKeyDown(MKey_I); break;
        case kVK_ANSI_J: _MWindowOnKeyDown(MKey_J); break;
        case kVK_ANSI_K: _MWindowOnKeyDown(MKey_K); break;
        case kVK_ANSI_L: _MWindowOnKeyDown(MKey_L); break;
        case kVK_ANSI_M: _MWindowOnKeyDown(MKey_M); break;
        case kVK_ANSI_N: _MWindowOnKeyDown(MKey_N); break;
        case kVK_ANSI_O: _MWindowOnKeyDown(MKey_O); break;
        case kVK_ANSI_P: _MWindowOnKeyDown(MKey_P); break;
        case kVK_ANSI_Q: _MWindowOnKeyDown(MKey_Q); break;
        case kVK_ANSI_R: _MWindowOnKeyDown(MKey_R); break;
        case kVK_ANSI_S: _MWindowOnKeyDown(MKey_S); break;
        case kVK_ANSI_T: _MWindowOnKeyDown(MKey_T); break;
        case kVK_ANSI_U: _MWindowOnKeyDown(MKey_U); break;
        case kVK_ANSI_V: _MWindowOnKeyDown(MKey_V); break;
        case kVK_ANSI_W: _MWindowOnKeyDown(MKey_W); break;
        case kVK_ANSI_X: _MWindowOnKeyDown(MKey_X); break;
        case kVK_ANSI_Y: _MWindowOnKeyDown(MKey_Y); break;
        case kVK_ANSI_Z: _MWindowOnKeyDown(MKey_Z); break;
        
        default:;
    }
    return nil;
}

@end
