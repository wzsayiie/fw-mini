#include "mimage.h"
#include "mfilemanager.h"
#include "mimagefactory.h"
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
