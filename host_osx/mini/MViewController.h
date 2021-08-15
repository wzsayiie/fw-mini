#import <Cocoa/Cocoa.h>

@interface MViewController : NSViewController <NSTextFieldDelegate>
@property (nonatomic) NSTextField *textField;
@property (nonatomic) BOOL textFieldEditing;
@end
