#include "mimage.h"
#include "mfilemanager.h"

//file format:

define_reflectable_enum_const(MImageFileFormat, JPEG)
define_reflectable_enum_const(MImageFileFormat, PNG )

//image:

define_reflectable_class_function(MImage, fromBundle, "args:path")
MImage::ptr MImage::fromBundle(const std::string &path) {
    MVector<uint8_t>::ptr bytes = MFileManager::instance()->bytesFromBundle(path);
    return fromFFData(bytes);
}

define_reflectable_class_function(MImage, fromFile, "args:path")
MImage::ptr MImage::fromFile(const std::string &path) {
    MVector<uint8_t>::ptr bytes = MFileManager::instance()->bytesFromFile(path);
    return fromFFData(bytes);
}

define_reflectable_class_function(MImage, fromFFData, "args:ffData")
MImage::ptr MImage::fromFFData(const MVector<uint8_t>::ptr &ffData) {
    if (!ffData || ffData->empty()) {
        return nullptr;
    }

    MImageImpl::ptr impl = MImageFactory::instance()->imageFromFFData(ffData);
    if (!impl) {
        return nullptr;
    }

    auto image = MImage::create();
    image->mImpl = impl;
    return image;
}

define_reflectable_class_function(MImage, fromBitmap, "args:bitmap,width,height")
MImage::ptr MImage::fromBitmap(const MVector<uint8_t>::ptr bitmap, int width, int height) {
    if (!bitmap || bitmap->empty()) {
        return nullptr;
    }

    if (width <= 0 || (int)bitmap->size() != width * height * 4) {
        return nullptr;
    }

    MImageImpl::ptr impl = MImageFactory::instance()->imageFromBitmap(bitmap, width, height);
    if (!impl) {
        return nullptr;
    }

    auto image = MImage::create();
    image->mImpl = impl;
    return image;
}

define_reflectable_class_function(MImage, writeFile, "args:path,format")
void MImage::writeFile(const std::string &path, MImageFileFormat format) {
    MVector<uint8_t>::ptr bytes = copyFFData(format);
    MFileManager::instance()->writeBytesToFile(bytes, path);
}

define_reflectable_class_function(MImage, copyFFData, "args:format")
MVector<uint8_t>::ptr MImage::copyFFData(MImageFileFormat format) {
    if (mImpl) {
        return MImageFactory::instance()->ffDataFromImage(mImpl, format);
    }
    return nullptr;
}

define_reflectable_class_function(MImage, copyBitmap)
MVector<uint8_t>::ptr MImage::copyBitmap() {
    if (mImpl) {
        return MImageFactory::instance()->bitmapFromImage(mImpl);
    }
    return nullptr;
}

define_reflectable_class_function(MImage, pixelSize)
MSize::ptr MImage::pixelSize() {
    if (mImpl) {
        return MImageFactory::instance()->pixelSize(mImpl);
    }
    return MSize::zero();
}

define_reflectable_class_function(MImage, impl)
MImageImpl::ptr MImage::impl() {
    return mImpl;
}

//image factory:

MImageFactory::ptr MImageFactory::sInstance;

define_reflectable_class_function(MImageFactory, setInstance, "args:obj")
void MImageFactory::setInstance(const MImageFactory::ptr &obj) {
    sInstance = obj;
}

define_reflectable_class_function(MImageFactory, instance)
MImageFactory *MImageFactory::instance() {
    if (!sInstance) {
        sInstance = MImageFactory::create();
    }
    return sInstance.get();
}

define_reflectable_class_function(MImageFactory, imageFromFFData, "args:ffData")
MImageImpl::ptr MImageFactory::imageFromFFData(const MVector<uint8_t>::ptr &ffData) {
    implement_injectable_function(MImageImpl::ptr, ffData)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, imageFromBitmap, "args:bitmap,width,height")
MImageImpl::ptr MImageFactory::imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) {
    implement_injectable_function(MImageImpl::ptr, bitmap, width, height)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, ffDataFromImage, "args:image,format")
MVector<uint8_t>::ptr MImageFactory::ffDataFromImage(const MImageImpl::ptr &impl, MImageFileFormat format) {
    implement_injectable_function(MVector<uint8_t>::ptr, impl, format)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, bitmapFromImage, "args:image")
MVector<uint8_t>::ptr MImageFactory::bitmapFromImage(const MImageImpl::ptr &impl) {
    implement_injectable_function(MVector<uint8_t>::ptr, impl)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, pixelSize, "args:image")
MSize::ptr MImageFactory::pixelSize(const MImageImpl::ptr &impl) {
    implement_injectable_function(MSize::ptr)
    return MSize::zero();
}
