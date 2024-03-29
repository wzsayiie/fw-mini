#import "MOSXViewController.h"
#import <Carbon/Carbon.h>
#import "MMACBundle.h"
#import "MMACDrawView.h"
#import "MMACImageFactory.h"
#import "MMACJavaScriptCore.h"
#import "MOSXFrameSaver.h"

const CGFloat TextFieldMaxWidth = 300;
const CGFloat TextFieldHeight   =  20;

@implementation MOSXViewController

- (NSTextField *)textField {
    if (!_textField) {
        _textField = [[NSTextField alloc] init];
        _textField.delegate = self;
        _textField.hidden = YES;
        
        [self.view addSubview:_textField];
        [self updateTextFieldFrame];
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
    
    NSRect frame = NSZeroRect;
    frame.size = MOSXFrameSaver.sharedSaver.contentSize;
    
    self.view = [[MMACDrawView alloc] initWithFrame:frame];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    MMACBundle::install();
    MMACImageFactory::install();
    MMACJsVM::install();
    
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
    [self addSelector:@selector(windowDidBecomeKey)   name:NSWindowDidBecomeKeyNotification  ];
    [self addSelector:@selector(windowDidMiniaturize) name:NSWindowDidMiniaturizeNotification];
    [self addSelector:@selector(windowWillClose)      name:NSWindowWillCloseNotification     ];
    [self addSelector:@selector(windowDidResize)      name:NSWindowDidResizeNotification     ];
    [self addSelector:@selector(windowDidMove)        name:NSWindowDidMoveNotification       ];
    
    //NOTE: the monitor will intercept ALL key down events.
    [NSEvent addLocalMonitorForEventsMatchingMask:NSEventMaskKeyDown handler:^NSEvent *(NSEvent *event) {
        return [self keyboardKeyDown:event];
    }];
}

- (void)addSelector:(SEL)sel name:(NSNotificationName)name {
    [NSNotificationCenter.defaultCenter addObserver:self selector:sel name:name object:nil];
}

- (void)windowDidBecomeKey {
    self.window->show();
}

- (void)windowDidMiniaturize {
    self.window->hide();
}

- (void)windowWillClose {
    //IMPORTANT: emit a "hide" event.
    //ensuring the lifecycle order of "load", to "show", to "hide".
    self.window->hide();
    
    [MOSXFrameSaver.sharedSaver save];
}

- (void)windowDidResize {
    [self updateTextFieldFrame];
    
    NSSize size = self.view.frame.size;
    self.window->resizePixel(size.width, size.height);
    
    MOSXFrameSaver.sharedSaver.contentSize = size;
}

- (void)windowDidMove {
    NSPoint origin = NSApplication.sharedApplication.keyWindow.frame.origin;
    MOSXFrameSaver.sharedSaver.windowOrigin = origin;
}

- (void)handleMouse:(NSEvent *)event step:(char)step {
    NSRect  bounds = self.view.bounds;
    NSPoint fromBL = [event locationInWindow];
    NSPoint fromUL = NSMakePoint(fromBL.x, bounds.size.height - fromBL.y);
    
    //mouse move event.
    //NOTE: on windows, if the mouse is moved out of the window, the mouse move message will not be received.
    //here is to ensure consistency across platforms.
    if (NSPointInRect(fromUL, bounds)) {
        auto mouse = MMouseMove::makePixel(fromUL.x, fromUL.y);
        self.window->mouseMove(mouse);
    }
    
    //touch event:
    MTouch::ptr touch;
    switch (step) {
        case 'B': touch = MTouch::makeBeginPixel(fromUL.x, fromUL.y, MTouchSource::LButton); break;
        case 'M': touch = MTouch::makeMovePixel (fromUL.x, fromUL.y, MTouchSource::LButton); break;
        case 'E': touch = MTouch::makeEndPixel  (fromUL.x, fromUL.y, MTouchSource::LButton); break;
    }
    if (!touch) {
        return;
    }
    
    MKbKey::ptr kbKey;
    switch (step) {
        case 'B': kbKey = MKbKey::make(MKbKeyCode::Null, [self modifiersOfEvent:event]); break;
    }
    
    self.window->touch(touch, kbKey);
}

- (void)mouseDown   :(NSEvent *)event { [self handleMouse:event step:'B']; }
- (void)mouseDragged:(NSEvent *)event { [self handleMouse:event step:'M']; }
- (void)mouseUp     :(NSEvent *)event { [self handleMouse:event step:'E']; }
- (void)mouseMoved  :(NSEvent *)event { [self handleMouse:event step:'_']; }

- (void)scrollWheel:(NSEvent *)event {
    NSRect  bounds = self.view.bounds;
    NSPoint fromBL = [event locationInWindow];
    NSPoint fromUL = NSMakePoint(fromBL.x, bounds.size.height - fromBL.y);
    
    //the value representation of the mouse wheel change is different on different platforms.
    //here is experience value.
    CGFloat delta = event.deltaY * 20;
    
    auto evt = MMouseWheel::makePixel(fromUL.x, fromUL.y, delta);
    self.window->mouseWheel(evt);
}

- (NSEvent *)keyboardKeyDown:(NSEvent *)event {
    if (!self.textField.hidden) {
        //text field key event:
        MKbKeyCode code = MKbKeyCode::Null;
        switch (event.keyCode) {
            case kVK_Tab   : code = MKbKeyCode::Tab  ; break;
            case kVK_Return: code = MKbKeyCode::Enter; break;
        }
        
        if (code != MKbKeyCode::Null) {
            auto evt = MKbKey::make(code, [self modifiersOfEvent:event]);
            self.window->kbKey(evt);
            return nil;
            
        } else {
            //the event needs to continue processing.
            return event;
        }
        
    } else {
        //key event:
        MKbKeyCode code = MKbKeyCode::Null;
        switch (event.keyCode) {
            case kVK_Delete    : code = MKbKeyCode::Back ; break;
            case kVK_Tab       : code = MKbKeyCode::Tab  ; break;
            case kVK_Return    : code = MKbKeyCode::Enter; break;
            case kVK_Space     : code = MKbKeyCode::Space; break;

            case kVK_LeftArrow : code = MKbKeyCode::Left ; break;
            case kVK_UpArrow   : code = MKbKeyCode::Up   ; break;
            case kVK_RightArrow: code = MKbKeyCode::Right; break;
            case kVK_DownArrow : code = MKbKeyCode::Down ; break;

            case kVK_ANSI_A    : code = MKbKeyCode::A    ; break;
            case kVK_ANSI_D    : code = MKbKeyCode::D    ; break;
            case kVK_ANSI_S    : code = MKbKeyCode::S    ; break;
            case kVK_ANSI_W    : code = MKbKeyCode::W    ; break;
        }
        
        if (code != MKbKeyCode::Null) {
            auto evt = MKbKey::make(code, [self modifiersOfEvent:event]);
            self.window->kbKey(evt);
            return nil;
            
        } else {
            return nil;
        }
    }
}

- (MKbModifiers)modifiersOfEvent:(NSEvent *)event {
    MKbModifiers modifiers = 0;
    
    if (event.modifierFlags & NSEventModifierFlagOption  ) { modifiers |= MKbModifier_Alt  ; }
    if (event.modifierFlags & NSEventModifierFlagCapsLock) { modifiers |= MKbModifier_Caps ; }
    if (event.modifierFlags & NSEventModifierFlagCommand ) { modifiers |= MKbModifier_Cmd  ; }
    if (event.modifierFlags & NSEventModifierFlagControl ) { modifiers |= MKbModifier_Ctrl ; }
    if (event.modifierFlags & NSEventModifierFlagShift   ) { modifiers |= MKbModifier_Shift; }
    
    return modifiers;
}

- (void)updateTextFieldFrame {
    NSSize space = self.view.frame.size;
    NSRect frame = NSZeroRect;
    
    frame.size.width  = space.width > TextFieldMaxWidth ? TextFieldMaxWidth : space.width;
    frame.size.height = TextFieldHeight;
    frame.origin.x    = (space.width  - frame.size.width ) / 2;
    frame.origin.y    = (space.height - frame.size.height) / 2;
    
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

- (void)controlTextDidChange:(NSNotification *)notification {
    std::string text(self.textField.stringValue.UTF8String);
    
    auto evt = MWriting::make(text);
    self.window->writing(evt);
}

@end
