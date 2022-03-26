#import <Cocoa/Cocoa.h>
#import "mapp.h"
#import "mwindow.h"

@interface MOSXViewController : NSViewController <NSTextFieldDelegate>
@property (nonatomic) NSTextField *textField;
@property (nonatomic) MApp *app;
@property (nonatomic) MWindow *window;
@end
