#include "mimage.h"
#include "mfilemanager.h"
#include "rdefine.h"

//file format:

define_reflectable_enum_const(MImageFileFormat, JPEG)
define_reflectable_enum_const(MImageFileFormat, PNG )

//image:

define_reflectable_class_function(MImage, fromBundle, "args:path")
MImage::ptr MImage::fromBundle(const std::string &path) {
    MData::ptr data = MFileManager::instance()->bytesFromBundle(path);
    return fromFFData(data);
}

define_reflectable_class_function(MImage, fromFile, "args:path")
MImage::ptr MImage::fromFile(const std::string &path) {
    MData::ptr data = MFileManager::instance()->bytesFromFile(path);
    return fromFFData(data);
}

define_reflectable_class_function(MImage, fromFFData, "args:ffData")
MImage::ptr MImage::fromFFData(const MData::ptr &ffData) {
    return MImageFactory::instance()->decodeFFData(ffData);
}

define_reflectable_class_function(MImage, fromBitmap, "args:bitmap,width,height")
MImage::ptr MImage::fromBitmap(const MData::ptr &bitmap, int width, int height) {
    return MImageFactory::instance()->decodeBitmap(bitmap, width, height);
}

define_reflectable_class_function(MImage, writeFile, "args:path,format")
void MImage::writeFile(const std::string &path, MImageFileFormat format) {
    MData::ptr data = copyFFData(format);
    MFileManager::instance()->writeBytesToFile(data, path);
}

define_reflectable_class_function(MImage, copyFFData, "args:format")
MData::ptr MImage::copyFFData(MImageFileFormat format) {
    return MImageFactory::instance()->encodeFFData(me(), format);
}

define_reflectable_class_function(MImage, copyBitmap)
MData::ptr MImage::copyBitmap() {
    return MImageFactory::instance()->encodeBitmap(me());
}

define_reflectable_class_function(MImage, pixelSize, "getter")
MSize::ptr MImage::pixelSize() {
    return MImageFactory::instance()->getPixelSize(me());
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
