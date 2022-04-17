#import <TargetConditionals.h>

#if TARGET_OS_OSX
    #import <AppKit/AppKit.h>
    @compatibility_alias _NSColor NSColor;
    @compatibility_alias _NSFont  NSFont ;
    @compatibility_alias _NSView  NSView ;

#elif TARGET_OS_IOS
    #import <UIKit/UIKit.h>
    @compatibility_alias _NSColor UIColor;
    @compatibility_alias _NSFont  UIFont ;
    @compatibility_alias _NSView  UIView ;

#endif

@interface MMACDrawView : _NSView
@end
