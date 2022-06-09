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
    if (ffData && !ffData->emplace_back()) {
        return MImageFactory::instance()->decodeFFData(ffData);
    }
    return nullptr;
}

define_reflectable_class_function(MImage, fromBitmap, "args:bitmap,width,height")
MImage::ptr MImage::fromBitmap(const MVector<uint8_t>::ptr bitmap, int width, int height) {
    if (bitmap && !bitmap->empty()) {
        return MImageFactory::instance()->decodeBitmap(bitmap, width, height);
    }
    return nullptr;
}

define_reflectable_class_function(MImage, writeFile, "args:path,format")
void MImage::writeFile(const std::string &path, MImageFileFormat format) {
    MVector<uint8_t>::ptr bytes = copyFFData(format);
    MFileManager::instance()->writeBytesToFile(bytes, path);
}

define_reflectable_class_function(MImage, copyFFData, "args:format")
MVector<uint8_t>::ptr MImage::copyFFData(MImageFileFormat format) {
    return MImageFactory::instance()->encodeFFData(shared(), format);
}

define_reflectable_class_function(MImage, copyBitmap)
MVector<uint8_t>::ptr MImage::copyBitmap() {
    return MImageFactory::instance()->encodeBitmap(shared());
}

define_reflectable_class_function(MImage, pixelSize, "getter")
MSize::ptr MImage::pixelSize() {
    return MImageFactory::instance()->getPixelSize(shared());
}

//image factory:

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
MImage::ptr MImageFactory::decodeFFData(const MVector<uint8_t>::ptr &ffData) {
    if (ffData && !ffData->empty()) {
        return onDecodeFFData(ffData);
    }
    return nullptr;
}

define_reflectable_class_function(MImageFactory, decodeBitmap, "args:bitmap,width,height")
MImage::ptr MImageFactory::decodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) {
    if (bitmap && width > 0 && (int)bitmap->size() == width * height) {
        return onDecodeBitmap(bitmap, width, height);
    }
    return nullptr;
}

define_reflectable_class_function(MImageFactory, encodeFFData, "args:image,format")
MVector<uint8_t>::ptr MImageFactory::encodeFFData(const MImage::ptr &image, MImageFileFormat format) {
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
MVector<uint8_t>::ptr MImageFactory::encodeBitmap(const MImage::ptr &image) {
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

MImage::ptr MImageFactory::onDecodeFFData(const MVector<uint8_t>::ptr &ffData) {
    return nullptr;
}

MImage::ptr MImageFactory::onDecodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) {
    return nullptr;
}

MVector<uint8_t>::ptr MImageFactory::onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) {
    return nullptr;
}

MVector<uint8_t>::ptr MImageFactory::onEncodeBitmap(const MImage::ptr &image) {
    return nullptr;
}

MSize::ptr MImageFactory::onGetPixelSize(const MImage::ptr &image) {
    return nullptr;
}
