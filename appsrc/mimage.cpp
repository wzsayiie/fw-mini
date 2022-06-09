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

    MVirtualImage::ptr real = MVirtualImageFactory::instance()->imageFromFFData(ffData);
    if (!real) {
        return nullptr;
    }

    auto image = MImage::create();
    image->mReal = real;
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

    MVirtualImage::ptr real = MVirtualImageFactory::instance()->imageFromBitmap(bitmap, width, height);
    if (!real) {
        return nullptr;
    }

    auto image = MImage::create();
    image->mReal = real;
    return image;
}

define_reflectable_class_function(MImage, writeFile, "args:path,format")
void MImage::writeFile(const std::string &path, MImageFileFormat format) {
    MVector<uint8_t>::ptr bytes = copyFFData(format);
    MFileManager::instance()->writeBytesToFile(bytes, path);
}

define_reflectable_class_function(MImage, copyFFData, "args:format")
MVector<uint8_t>::ptr MImage::copyFFData(MImageFileFormat format) {
    if (mReal) {
        return MVirtualImageFactory::instance()->ffDataFromImage(mReal, format);
    }
    return nullptr;
}

define_reflectable_class_function(MImage, copyBitmap)
MVector<uint8_t>::ptr MImage::copyBitmap() {
    if (mReal) {
        return MVirtualImageFactory::instance()->bitmapFromImage(mReal);
    }
    return nullptr;
}

define_reflectable_class_function(MImage, pixelSize, "getter")
MSize::ptr MImage::pixelSize() {
    if (mReal) {
        return MVirtualImageFactory::instance()->pixelSize(mReal);
    }
    return MSize::zero();
}

define_reflectable_class_function(MImage, real, "getter")
MVirtualImage::ptr MImage::real() {
    return mReal;
}

//image factory:

MVirtualImageFactory::ptr MVirtualImageFactory::sInstance;

void MVirtualImageFactory::setInstance(const MVirtualImageFactory::ptr &obj) {
    sInstance = obj;
}

MVirtualImageFactory *MVirtualImageFactory::instance() {
    if (!sInstance) {
        sInstance = MVirtualImageFactory::create();
    }
    return sInstance.get();
}

MVirtualImage::ptr MVirtualImageFactory::imageFromFFData(const MVector<uint8_t>::ptr &ffData) {
    return nullptr;
}

MVirtualImage::ptr MVirtualImageFactory::imageFromBitmap(
    const MVector<uint8_t>::ptr &bitmap, int width, int height)
{
    return nullptr;
}

MVector<uint8_t>::ptr MVirtualImageFactory::ffDataFromImage(
    const MVirtualImage::ptr &real, MImageFileFormat format)
{
    return nullptr;
}

MVector<uint8_t>::ptr MVirtualImageFactory::bitmapFromImage(const MVirtualImage::ptr &real) {
    return nullptr;
}

MSize::ptr MVirtualImageFactory::pixelSize(const MVirtualImage::ptr &real) {
    return MSize::zero();
}
