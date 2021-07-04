#import "MWindowController.h"
#import "MViewController.h"

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
