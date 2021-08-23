#import "MViewController.h"
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

- (void)mouseEvent:(NSEvent *)event func:(void (*)(float, float))func {
    NSPoint point = [event locationInWindow];
    func(point.x, self.view.frame.size.height - point.y);
}

- (void)mouseDown   :(NSEvent *)event { [self mouseEvent:event func:_MWindowOnTouchBegin]; }
- (void)mouseDragged:(NSEvent *)event { [self mouseEvent:event func:_MWindowOnTouchMove ]; }
- (void)mouseUp     :(NSEvent *)event { [self mouseEvent:event func:_MWindowOnTouchEnd  ]; }

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
    } else {
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
            default :;
        }
        return nil;
    }
}

@end
