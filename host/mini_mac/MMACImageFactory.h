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
    MImage::ptr onDecodeFFData(const MData::ptr &ffData) override;
    MImage::ptr onDecodeBitmap(const MData::ptr &bitmap, int width, int height) override;

    MData::ptr onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) override;
    MData::ptr onEncodeBitmap(const MImage::ptr &image) override;

    MSize::ptr onGetPixelSize(const MImage::ptr &image) override;
};
