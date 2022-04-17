#import <TargetConditionals.h>
#import "mimage.h"

#if TARGET_OS_OSX
    #import <AppKit/AppKit.h>
    @compatibility_alias _NSImage NSImage;

#elif TARGET_OS_IOS
    #import <UIKit/UIKit.h>
    @compatibility_alias _NSImage UIImage;

#endif

declare_reflectable_class(MMACImageImpl)
class MMACImageImpl : public MExtends<MMACImageImpl, MImageImpl> {
public:
    _NSImage *mReal = nil;
};

declare_reflectable_class(MMACImageFactory)
class MMACImageFactory : public MExtends<MMACImageFactory, MImageFactory> {
public:
    static void install();
    
public:
    MImageImpl::ptr imageFromFFData(const MVector<uint8_t>::ptr &ffData) override;
    MImageImpl::ptr imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) override;

    MVector<uint8_t>::ptr ffDataFromImage(const MImageImpl::ptr &impl, MImageFileFormat format) override;
    MVector<uint8_t>::ptr bitmapFromImage(const MImageImpl::ptr &impl) override;

    MSize::ptr pixelSize(const MImageImpl::ptr &impl) override;
};
