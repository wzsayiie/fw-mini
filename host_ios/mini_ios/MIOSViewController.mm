#import "MIOSViewController.h"
#import "MMACBundle.h"
#import "MMACDrawView.h"
#import "MMACImageFactory.h"
#import "MMACJavaScriptCore.h"

const CGFloat TextFieldWidth  = 300;
const CGFloat TextFieldHeight =  30;
const CGFloat TextFieldBottom =  20;

@implementation MIOSViewController

- (UITextField *)textField {
    if (!_textField) {
        _textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, TextFieldWidth, TextFieldHeight)];
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

- (void)touches:(NSSet<UITouch *> *)touches func:(void (MWindow::*)(float x, float y))func {
    CGPoint point = [touches.anyObject locationInView:self.view];
    (self.window->*func)(point.x, point.y);
}

- (void)touchesBegan    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t func:&MWindow::touchBeginPixel]; }
- (void)touchesMoved    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t func:&MWindow::touchMovePixel ]; }
- (void)touchesEnded    :(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t func:&MWindow::touchEndPixel  ]; }
- (void)touchesCancelled:(NSSet<UITouch *> *)t withEvent:(UIEvent *)e { [self touches:t func:&MWindow::touchEndPixel  ]; }

- (void)keyboardWillShow:(NSNotification *)notification {
    CGRect rect = [[notification.userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    [self updateTextFieldFrameWithSpace:self.view.frame.size keyboardHeight:rect.size.height];
}

- (void)keyboardWillHide:(NSNotification *)notification {
    [self updateTextFieldFrameWithSpace:self.view.frame.size keyboardHeight:0];
}

- (void)updateTextFieldFrameWithSpace:(CGSize)space keyboardHeight:(CGFloat)keyboardHeight {
    CGRect frame = self.textField.frame;
    
    frame.origin.x = (space.width  - frame.size.width ) / 2;
    frame.origin.y = (space.height - frame.size.height) - TextFieldBottom - keyboardHeight;
    
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
    self.window->write(text, false);
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    std::string text(textField.text.UTF8String);
    self.window->write(text, true);
    return YES;
}

@end
