#import "MOSXViewController.h"

#pragma mark - window controller

@interface MOSXWindowController : NSWindowController
@end

@implementation MOSXWindowController

+ (MOSXWindowController *)windowController {
    //only assign the origin, the size will be taken care of by content view.
    NSRect rect = NSMakeRect(1000, 200, 0, 0);
    
    NSWindowStyleMask  style   = self.windowStyle;
    NSBackingStoreType backing = NSBackingStoreBuffered;
    
    NSWindow *window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:backing defer:NO];
    window.contentViewController = [[MOSXViewController alloc] init];
    window.acceptsMouseMovedEvents = YES;
    window.title = @(MWindow::TitleName);
    
    MOSXWindowController *controller = [[MOSXWindowController alloc] initWithWindow:window];
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

@interface MOSXAppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic) MOSXWindowController *windowController;
@end

@implementation MOSXAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    self.windowController = [MOSXWindowController windowController];
    [self.windowController showWindow:nil];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES;
}

@end

#pragma mark - main

extern "C" void MOSXAppMain() {
    Class clazz = NSClassFromString(@"MOSXAppDelegate");
    id<NSApplicationDelegate> delegate = [[clazz alloc] init];
    
    //NOTE: the "delegate" is weak. user needs to keep its reference count.
    NSApplication.sharedApplication.delegate = delegate;
    
    //NSApplicationMain() will try to read the info.plist.
    //if it is a custom directory structure, this will cause inconvenience.
    [NSApplication.sharedApplication run];
}

int main() {
    MOSXAppMain();
}
