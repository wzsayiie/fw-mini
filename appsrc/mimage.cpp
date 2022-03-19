#include "mimage.h"

//image factory:
//

MImageFactory::ptr MImageFactory::sFactory;

define_reflectable_class_function(MImageFactory, setSharedObject, "args:factory")
void MImageFactory::setSharedObject(const MImageFactory::ptr &factory) {
    sFactory = factory;
}

define_reflectable_class_function(MImageFactory, sharedObject)
MImageFactory *MImageFactory::sharedObject() {
    if (!sFactory) {
        sFactory = MImageFactory::create();
    }
    return sFactory.get();
}

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
