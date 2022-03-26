#import <UIKit/UIKit.h>
#import "mapp.h"
#import "mwindow.h"

@interface MIOSViewController : UIViewController <UITextFieldDelegate>
@property (nonatomic) UITextField *textField;
@property (nonatomic) MApp *app;
@property (nonatomic) MWindow *window;
@end
