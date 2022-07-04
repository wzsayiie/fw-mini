#import <TargetConditionals.h>
#import "mimage.h"

#if TARGET_OS_OSX
    #import <AppKit/AppKit.h>
    @compatibility_alias _NSImage NSImage;

#elif TARGET_OS_IOS
    #import <UIKit/UIKit.h>
    @compatibility_alias _NSImage UIImage;

#endif

m_class(MMACImage, MImage) {
public:
    _NSImage *mNSImage = nil;
};

m_class(MMACImageFactory, MImageFactory) {
public:
    static void install();
    
public:
    MImage::ptr onDecodeFFData(const MVector<uint8_t>::ptr &ffData) override;
    MImage::ptr onDecodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) override;

    MVector<uint8_t>::ptr onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) override;
    MVector<uint8_t>::ptr onEncodeBitmap(const MImage::ptr &image) override;

    MSize::ptr onGetPixelSize(const MImage::ptr &image) override;
};
