#import "MViewController.h"
#import "MIOSAPI.h"
#import "MJavaScriptCore.h"
#import "MView.h"
#import "mhostloop.h"
#import "mhostui.h"

@implementation MViewController

- (void)loadView {
    self.view = [[MView alloc] init];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    MInstallJSVirtualMachine();
    MRegisterAPIs();
    
    CGSize size = UIScreen.mainScreen.bounds.size;
    [self adjusteTextFieldFrameWithSuperSize:size keyboardHeight:0];
    
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
        [self.view setNeedsDisplay];
    }];
    
    NSNotificationCenter *center = NSNotificationCenter.defaultCenter;
    [center addObserver:self selector:@selector(applicationWillEnterForeground) name:UIApplicationWillEnterForegroundNotification object:nil];
    [center addObserver:self selector:@selector(applicationWillResignActive)    name:UIApplicationWillResignActiveNotification    object:nil];
    
    [center addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
    [center addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
}

- (void)applicationWillEnterForeground {
    _MWindowOnShow();
}

- (void)applicationWillResignActive {
    _MWindowOnHide();
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    [self adjusteTextFieldFrameWithSuperSize:size keyboardHeight:0];
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

- (void)keyboardWillShow:(NSNotification *)notification {
    CGRect rect = [[notification.userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    [self adjusteTextFieldFrameWithSuperSize:self.view.frame.size keyboardHeight:rect.size.height];
}

- (void)keyboardWillHide:(NSNotification *)notification {
    [self adjusteTextFieldFrameWithSuperSize:self.view.frame.size keyboardHeight:0];
}

- (UITextField *)textField {
    if (!_textField) {
        
        _textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, 300, 30)];
        _textField.layer.masksToBounds = YES;
        _textField.layer.cornerRadius = 5;
        _textField.layer.borderWidth = 1;
        _textField.delegate = self;
        _textField.hidden = YES;
        
        [self.view addSubview:_textField];
    }
    return _textField;
}

- (void)adjusteTextFieldFrameWithSuperSize:(CGSize)superSize keyboardHeight:(CGFloat)keyboardHeight {
    CGRect frame = self.textField.frame;
    
    frame.origin.x = (superSize.width - frame.size.width) / 2;
    frame.origin.y = superSize.height - keyboardHeight - 60;
    
    self.textField.frame = frame;
}

- (void)updateTextFieldState {
    if (!_MWindowTextBoxUpdated()) {
        return;
    }
    
    //IMPORTANT: reset the flag;
    MWindowSetTextBoxUpdated(false);
    
    if (_MWindowTextBoxEnabled()) {
        MString  *raw  = _MWindowTextBoxRawString();
        NSString *text = [NSString stringWithUTF8String:MStringU8Chars(raw)];
        
        [self.textField setText:text];
        [self.textField selectAll:nil];
        [self.textField setHidden:false];
        [self.textField becomeFirstResponder];
        
    } else {
        [self.textField resignFirstResponder];
        [self.textField setHidden:true];
    }
}

- (void)textFieldDidChangeSelection:(UITextField *)textField {
    MStringRef string = m_auto_release MStringCreateU8(textField.text.UTF8String);
    _MWindowOnTextBox(string.get(), false);
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    MStringRef string = m_auto_release MStringCreateU8(textField.text.UTF8String);
    _MWindowOnTextBox(string.get(), true);
    return YES;
}

@end
