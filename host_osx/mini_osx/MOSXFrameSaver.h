#import <AppKit/AppKit.h>

@interface MOSXFrameSaver : NSObject

+ (instancetype)sharedSaver;

@property (nonatomic) NSPoint windowOrigin;
@property (nonatomic) NSSize  contentSize ;
- (void)save;

@end
