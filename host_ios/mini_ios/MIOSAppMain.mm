#import "MIOSViewController.h"

#pragma mark - application delegate

@interface MAppDelegate : UIResponder <UIApplicationDelegate>
@property (nonatomic) UIWindow *window;
@end

@implementation MAppDelegate

- (BOOL) /* ...... */ application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey, id> *)options
{
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = [[MIOSViewController alloc] init];
    [self.window makeKeyAndVisible];

    return YES;
}

@end

#pragma mark - main

int main(int argc, char *argv[]) {
    return UIApplicationMain(argc, argv, nil, @"MAppDelegate");
}
