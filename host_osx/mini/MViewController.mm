#import "MViewController.h"
#import "mmain.h"

@interface MViewController ()
@end

@implementation MViewController

- (void)loadView {
    //NOTE: don't call [super loadView], that will try to unarchive a nib.
    self.view = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 300, 200)];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    _MMain();
}

@end
