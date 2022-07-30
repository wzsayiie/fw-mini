#import "MIOSViewController.h"
#import "MMACBundle.h"
#import "MMACDrawView.h"
#import "MMACImageFactory.h"
#import "MMACJavaScriptCore.h"

@implementation MIOSViewController

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
    self.view = [[MMACDrawView alloc] init];
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
    CGSize size = UIScreen.mainScreen.bounds.size;
    self.window->resizePixel(size.width, size.height);
    self.window->load();
    
    [self updateTextFieldFrameWithSpace:size keyboardHeight:0];
    [NSTimer scheduledTimerWithTimeInterval:MWindow::UpdateEverySeconds repeats:YES block:^(NSTimer *timer) {
        [self.view setNeedsDisplay];
        [self updateTextFieldState];
    }];
    
    //notifications listening:
    NSNotificationCenter *center = NSNotificationCenter.defaultCenter;
    [center addObserver:self selector:@selector(applicationWillEnterForeground) name:UIApplicationWillEnterForegroundNotification object:nil];
    [center addObserver:self selector:@selector(applicationWillResignActive)    name:UIApplicationWillResignActiveNotification    object:nil];
    
    [center addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
    [center addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    //this is a single-controller app,
    //this event will only be fired once on launching.
    self.window->show();
}

- (void)applicationWillEnterForeground {
    self.window->show();
}

- (void)applicationWillResignActive {
    self.window->hide();
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    [self updateTextFieldFrameWithSpace:size keyboardHeight:0];
    self.window->resizePixel(size.width, size.height);
}

- (void)touches:(NSSet<UITouch *> *)touches step:(char)step {
    CGPoint point = [touches.anyObject locationInView:self.view];
    
    MTouch::ptr evt;
    switch (step) {
        case 'B': evt = MTouch::makeBeginPixel(point.x, point.y, MTouchSource::Finger); break;
        case 'M': evt = MTouch::makeMovePixel (point.x, point.y, MTouchSource::Finger); break;
        case 'E': evt = MTouch::makeEndPixel  (point.x, point.y, MTouchSource::Finger); break;
    }
    self.window->touch(evt, nullptr);
}

- (void)touchesBegan    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t step:'B']; }
- (void)touchesMoved    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t step:'M']; }
- (void)touchesEnded    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t step:'E']; }
- (void)touchesCancelled:(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t step:'E']; }

- (void)keyboardWillShow:(NSNotification *)notification {
    CGRect rect = [[notification.userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    [self updateTextFieldFrameWithSpace:self.view.frame.size keyboardHeight:rect.size.height];
}

- (void)keyboardWillHide:(NSNotification *)notification {
    [self updateTextFieldFrameWithSpace:self.view.frame.size keyboardHeight:0];
}

- (void)updateTextFieldFrameWithSpace:(CGSize)space keyboardHeight:(CGFloat)keyboardHeight {
    space.height -= keyboardHeight;
    
    CGRect frame = self.textField.frame;
    frame.origin.x = (space.width  - frame.size.width ) / 2;
    frame.origin.y = (space.height - frame.size.height) / 2;
    
    self.textField.frame = frame;
}

- (void)updateTextFieldState {
    if (!self.window->checkWritingUpdated()) {
        return;
    }
    
    if (self.window->checkWritingEnabled()) {
        [self.textField setText:@(self.window->checkWritingRawText().c_str())];
        [self.textField selectAll:nil];
        [self.textField setHidden:false];
        [self.textField becomeFirstResponder];
        
    } else {
        [self.textField resignFirstResponder];
        [self.textField setHidden:true];
    }
}

- (void)textFieldDidChangeSelection:(UITextField *)textField {
    std::string text(textField.text.UTF8String);
    
    auto evt = MWriting::make(text);
    self.window->writing(evt);
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    auto evt = MKbKey::make(MKbKeyCode::Enter, 0);
    self.window->kbKey(evt);
    
    return YES;
}

@end
