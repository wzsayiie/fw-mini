#import "MAppDelegate.h"
#import "MWindowController.h"

@interface MAppDelegate ()
@property (nonatomic) MWindowController *windowController;
@end

@implementation MAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    self.windowController = [MWindowController windowController];
    [self.windowController showWindow:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES;
}

@end
