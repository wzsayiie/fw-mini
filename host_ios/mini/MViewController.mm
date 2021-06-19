#import "MViewController.h"
#import "mapp.h"

@interface MViewController ()
@end

@implementation MViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor colorWithRed:0.4 green:0.6 blue:1 alpha:1];
    
    _MAppLaunch();
}

@end
