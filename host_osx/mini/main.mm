#import "MViewController.h"

#pragma mark - window controller

@interface MWindowController : NSWindowController
@end

@implementation MWindowController

+ (MWindowController *)windowController {
    //only assign the origin, the size will be taken care of by content view.
    NSRect rect = NSMakeRect(600, 200, 0, 0);
    
    NSWindowStyleMask style = self.windowStyle;
    NSBackingStoreType backing = NSBackingStoreBuffered;
    NSWindow *window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:backing defer:NO];
    [window setTitle:@"mini"];
    
    MWindowController *controller = [[MWindowController alloc] initWithWindow:window];
    window.contentViewController = [[MViewController alloc] init];
    return controller;
}

+ (NSWindowStyleMask)windowStyle {
    NSWindowStyleMask style = 0;
    style |= NSWindowStyleMaskTitled;
    style |= NSWindowStyleMaskClosable;
    style |= NSWindowStyleMaskMiniaturizable;
    style |= NSWindowStyleMaskResizable;
    return style;
}

@end

#pragma mark - application delegate

@interface MAppDelegate : NSObject <NSApplicationDelegate>
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

#pragma mark - main

int main(int argc, const char *argv[]) {
    
    Class clazz = NSClassFromString(@"MAppDelegate");
    id<NSApplicationDelegate> delegate = [[clazz alloc] init];
    
    //NOTE: the "delegate" is weak.
    NSApplication.sharedApplication.delegate = delegate;
    return NSApplicationMain(argc, argv);
}
