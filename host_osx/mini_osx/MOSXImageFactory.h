#import <AppKit/AppKit.h>
#import "mimage.h"

declare_reflectable_class(MOSXImageImpl)
class MOSXImageImpl : public MExtends<MOSXImageImpl, MImageImpl> {
public:
    NSImage *mImage = nil;
};

declare_reflectable_class(MOSXImageFactory)
class MOSXImageFactory : public MExtends<MOSXImageFactory, MImageFactory> {
public:
    static void install();
    
public:
    MImageImpl::ptr imageFromFFData(const MVector<uint8_t>::ptr &ffData) override;
    MImageImpl::ptr imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) override;

    MVector<uint8_t>::ptr ffDataFromImage(const MImageImpl::ptr &image, MImageFileFormat format) override;
    MVector<uint8_t>::ptr bitmapFromImage(const MImageImpl::ptr &image) override;

    MSize::ptr pixelSize(const MImageImpl::ptr &image) override;
};
