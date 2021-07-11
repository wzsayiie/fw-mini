#import "MAPIs.h"
#import "mhostapis.h"

@interface ManagedObjectPool : NSObject
@property (nonatomic) NSMutableDictionary<NSNumber *, NSObject *> *objects;
@property (nonatomic) int IDCount;
@end

@implementation ManagedObjectPool

- (instancetype)init {
    if (self = [super init]) {
        self.objects = [NSMutableDictionary dictionary];
    }
    return self;
}

- (int)addObject:(NSObject *)object {
    if (!object) {
        return 0;
    }
    
    int ID = ++self.IDCount;
    self.objects[@(ID)] = object;
    return ID;
}

- (NSObject *)objectForID:(int)ID {
    return self.objects[@(ID)];
}

- (void)removeObjectForID:(int)ID {
    [self.objects removeObjectForKey:@(ID)];
}

+ (ManagedObjectPool *)managedImages {
    static ManagedObjectPool *pool = nil;
    if (!pool) {
        pool = [[ManagedObjectPool alloc] init];
    }
    return pool;
}

@end

NSImage *MManagedImage(int ID) {
    return (NSImage *)[ManagedObjectPool.managedImages objectForID:ID];
}

void MRegisterAPIs() {
}
