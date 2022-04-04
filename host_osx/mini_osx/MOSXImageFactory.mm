#include "MOSXImageFactory.h"

void MOSXImageFactory::install() {
    auto obj = MOSXImageFactory::create();
    setSharedObject(obj);
}

MImageImpl::ptr MOSXImageFactory::imageFromFFData(const MVector<uint8_t>::ptr &ffData) {
    return nullptr;
}

MImageImpl::ptr MOSXImageFactory::imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) {
    return nullptr;
}

MVector<uint8_t>::ptr MOSXImageFactory::ffDataFromImage(const MImageImpl::ptr &image, MImageFileFormat format) {
    return nullptr;
}

MVector<uint8_t>::ptr MOSXImageFactory::bitmapFromImage(const MImageImpl::ptr &image) {
    return nullptr;
}

MSize::ptr MOSXImageFactory::pixelSize(const MImageImpl::ptr &image) {
    return MSize::create();
}
