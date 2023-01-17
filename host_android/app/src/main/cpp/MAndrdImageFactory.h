#pragma once

#include <jni.h>
#include "mimagefactory.h"

m_class(MAndrdImage, MImage) {
public:
    std::shared_ptr<_jobject> mBitmap;
};

m_class(MAndrdImageFactory, MImageFactory) {
protected:
    MImage::ptr onDecodeFFData(const MData::ptr &ffData) override;
    MImage::ptr onDecodeBitmap(const MData::ptr &bitDat, int width, int height) override;

    MData::ptr onEncodeFFData(const MImage::ptr &image, MImageFileFormat format) override;
    MData::ptr onEncodeBitmap(const MImage::ptr &image) override;

    MSize::ptr onGetPixelSize(const MImage::ptr &image) override;
};
