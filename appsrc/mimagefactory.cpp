#include "mimagefactory.h"
#include "rdefine.h"

MImageFactory::ptr MImageFactory::sInstance;

define_reflectable_class_function(MImageFactory, setInstance, "setter;args:obj")
void MImageFactory::setInstance(const MImageFactory::ptr &obj) {
    sInstance = obj;
}

define_reflectable_class_function(MImageFactory, instance, "getter")
MImageFactory *MImageFactory::instance() {
    if (!sInstance) {
        sInstance = MImageFactory::create();
    }
    return sInstance.get();
}

define_reflectable_class_function(MImageFactory, decodeFFData, "args:ffData")
MImage::ptr MImageFactory::decodeFFData(const MData::ptr &ffData) {
    if (ffData && !ffData->empty()) {
        return onDecodeFFData(ffData);
    }
    return nullptr;
}

define_reflectable_class_function(MImageFactory, decodeBitmap, "args:bitmap,width,height")
MImage::ptr MImageFactory::decodeBitmap(const MData::ptr &bitmap, int width, int height) {
    if (bitmap && width > 0 && bitmap->length() == width * height * 4) {
        return onDecodeBitmap(bitmap, width, height);
    }
    return nullptr;
}

define_reflectable_class_function(MImageFactory, encodeFFData, "args:image,format")
MData::ptr MImageFactory::encodeFFData(const MImage::ptr &image, MImageFileFormat format) {
    if (format != MImageFileFormat::JPEG &&
        format != MImageFileFormat::PNG  )
    {
        return nullptr;
    }
    if (!image) {
        return nullptr;
    }

    return onEncodeFFData(image, format);
}

define_reflectable_class_function(MImageFactory, encodeBitmap, "args:image")
MData::ptr MImageFactory::encodeBitmap(const MImage::ptr &image) {
    if (image) {
        return onEncodeBitmap(image);
    }
    return nullptr;
}

define_reflectable_class_function(MImageFactory, getPixelSize, "args:image")
MSize::ptr MImageFactory::getPixelSize(const MImage::ptr &image) {
    if (image) {
        MSize::ptr size = onGetPixelSize(image);
        return size ? size : MSize::zero();
    }
    return MSize::zero();
}

MImage::ptr MImageFactory::onDecodeFFData(const MData::ptr &ffData) {
    return nullptr;
}

MImage::ptr MImageFactory::onDecodeBitmap(const MData::ptr &bitmap, int width, int height) {
    return nullptr;
}

MData::ptr MImageFactory::onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) {
    return nullptr;
}

MData::ptr MImageFactory::onEncodeBitmap(const MImage::ptr &image) {
    return nullptr;
}

MSize::ptr MImageFactory::onGetPixelSize(const MImage::ptr &image) {
    return nullptr;
}
