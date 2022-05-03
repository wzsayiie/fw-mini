#include "mimage.h"
#include "mfilemanager.h"

//file format:

define_reflectable_enum_const(MImageFileFormat, JPEG)
define_reflectable_enum_const(MImageFileFormat, PNG )

//image:

define_reflectable_class_function(MImage, imageFromBundle, "args:path")
MImage::ptr MImage::imageFromBundle(const std::string &path) {
    MVector<uint8_t>::ptr bytes = MFileManager::sharedObject()->bytesFromBundle(path);
    return imageFromFFData(bytes);
}

define_reflectable_class_function(MImage, imageFromFile, "args:path")
MImage::ptr MImage::imageFromFile(const std::string &path) {
    MVector<uint8_t>::ptr bytes = MFileManager::sharedObject()->bytesFromFile(path);
    return imageFromFFData(bytes);
}

define_reflectable_class_function(MImage, imageFromFFData, "args:ffData")
MImage::ptr MImage::imageFromFFData(const MVector<uint8_t>::ptr &ffData) {
    if (!ffData || ffData->vector.empty()) {
        return nullptr;
    }

    MImageImpl::ptr impl = MImageFactory::sharedObject()->imageFromFFData(ffData);
    if (!impl) {
        return nullptr;
    }

    auto image = MImage::create();
    image->mImpl = impl;
    return image;
}

define_reflectable_class_function(MImage, imageFromBitmap, "args:bitmap,width,height")
MImage::ptr MImage::imageFromBitmap(const MVector<uint8_t>::ptr bitmap, int width, int height) {
    if (!bitmap || bitmap->vector.empty()) {
        return nullptr;
    }

    if (width <= 0 || (int)bitmap->vector.size() != width * height * 4) {
        return nullptr;
    }

    MImageImpl::ptr impl = MImageFactory::sharedObject()->imageFromBitmap(bitmap, width, height);
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
    MFileManager::sharedObject()->writeBytesToFile(bytes, path);
}

define_reflectable_class_function(MImage, copyFFData, "args:format")
MVector<uint8_t>::ptr MImage::copyFFData(MImageFileFormat format) {
    if (mImpl) {
        return MImageFactory::sharedObject()->ffDataFromImage(mImpl, format);
    }
    return nullptr;
}

define_reflectable_class_function(MImage, copyBitmap)
MVector<uint8_t>::ptr MImage::copyBitmap() {
    if (mImpl) {
        return MImageFactory::sharedObject()->bitmapFromImage(mImpl);
    }
    return nullptr;
}

define_reflectable_class_function(MImage, pixelSize)
MSize::ptr MImage::pixelSize() {
    if (mImpl) {
        return MImageFactory::sharedObject()->pixelSize(mImpl);
    }
    return MSize::zero();
}

define_reflectable_class_function(MImage, impl)
MImageImpl::ptr MImage::impl() {
    return mImpl;
}

//image factory:

MImageFactory::ptr MImageFactory::sSharedObject;

define_reflectable_class_function(MImageFactory, setSharedObject, "args:obj")
void MImageFactory::setSharedObject(const MImageFactory::ptr &obj) {
    sSharedObject = obj;
}

define_reflectable_class_function(MImageFactory, sharedObject)
MImageFactory *MImageFactory::sharedObject() {
    if (!sSharedObject) {
        sSharedObject = MImageFactory::create();
    }
    return sSharedObject.get();
}

define_reflectable_class_function(MImageFactory, imageFromFFData, "args:ffData")
MImageImpl::ptr MImageFactory::imageFromFFData(const MVector<uint8_t>::ptr &ffData) {
    implement_injectable_function((MImageImpl::ptr), ffData)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, imageFromBitmap, "args:bitmap,width,height")
MImageImpl::ptr MImageFactory::imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) {
    implement_injectable_function((MImageImpl::ptr), bitmap, width, height)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, ffDataFromImage, "args:image,format")
MVector<uint8_t>::ptr MImageFactory::ffDataFromImage(const MImageImpl::ptr &impl, MImageFileFormat format) {
    implement_injectable_function((MVector<uint8_t>::ptr), impl, (int)format)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, bitmapFromImage, "args:image")
MVector<uint8_t>::ptr MImageFactory::bitmapFromImage(const MImageImpl::ptr &impl) {
    implement_injectable_function((MVector<uint8_t>::ptr), impl)
    return nullptr;
}

define_reflectable_class_function(MImageFactory, pixelSize, "args:image")
MSize::ptr MImageFactory::pixelSize(const MImageImpl::ptr &impl) {
    implement_injectable_function((MSize::ptr))
    return MSize::zero();
}
