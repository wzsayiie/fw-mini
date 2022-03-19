#include "mimage.h"

define_reflectable_class_function(MImageFactory, imageFromFFData, "args:ffData;")
MImageImpl::ptr MImageFactory::imageFromFFData(const MVector<uint8_t>::ptr ffData) {
    implement_injectable_function((MImageImpl::ptr), ffData)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, imageFromBitmap, "args:bitmap,width,height;")
MImageImpl::ptr MImageFactory::imageFromBitmap(const MVector<uint8_t>::ptr bitmap, int width, int height) {
    implement_injectable_function((MImageImpl::ptr), bitmap, width, height)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, ffDataFromImage, "args:image;")
MVector<uint8_t>::ptr MImageFactory::ffDataFromImage(const MImageImpl::ptr image) {
    implement_injectable_function((MVector<uint8_t>::ptr), image)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, bitmapFromImage, "args:image;")
MVector<uint8_t>::ptr MImageFactory::bitmapFromImage(const MImageImpl::ptr image) {
    implement_injectable_function((MVector<uint8_t>::ptr), image)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, sizePixel)
MSize::ptr MImageFactory::sizePixel() {
    implement_injectable_function((MSize::ptr))
    return nullptr;
}

static dash::lazy_var<MImageFactory::ptr> sImageFactory;

void MSetImageFactory(const MImageFactory::ptr &factory) {
    sImageFactory = factory;
}

MImageFactory *MGetImageFactory() {
    if (!sImageFactory) {
        sImageFactory = MImageFactory::create();
    }
    return sImageFactory;
}
