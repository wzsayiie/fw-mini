#import "MWindowController.h"
#import "MViewController.h"

@interface MWindowController () <NSWindowDelegate>
@end

@implementation MWindowController

+ (MWindowController *)windowController {
    //onyl assign the origin, the size will be taken care of by contentView.
    NSRect rect = NSMakeRect(100, 500, 0, 0);
    
    NSWindowStyleMask style = 0;
    style |= NSWindowStyleMaskTitled;
    style |= NSWindowStyleMaskClosable;
    style |= NSWindowStyleMaskMiniaturizable;
    style |= NSWindowStyleMaskResizable;
    
    NSBackingStoreType backing = NSBackingStoreBuffered;
    
    NSWindow *window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:backing defer:NO];
    MWindowController *controller = [[MWindowController alloc] initWithWindow:window];
    window.contentViewController = [[MViewController alloc] init];
    window.delegate = controller;
    
    return controller;
}

@end
