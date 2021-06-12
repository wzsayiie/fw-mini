#import <Cocoa/Cocoa.h>

int main(int argc, const char *argv[]) {
    
    Class clazz = NSClassFromString(@"MAppDelegate");
    id<NSApplicationDelegate> delegate = [[clazz alloc] init];
    
    //NOTE: the "delegate" is weak.
    NSApplication.sharedApplication.delegate = delegate;
    return NSApplicationMain(argc, argv);
}
