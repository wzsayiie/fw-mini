#import "MAppDelegate.h"
#import "MViewController.h"

@implementation MAppDelegate

- (BOOL) /* ====== */ application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey, id> *)options
{
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = [[MViewController alloc] init];
    [self.window makeKeyAndVisible];

    return YES;
}

@end
