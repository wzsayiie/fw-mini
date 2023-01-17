#pragma once

#include "mimage.h"

m_class(MImageFactory, MObject) {
public:
    M_HOST_CALL static void setInstance(const MImageFactory::ptr &obj);
    static MImageFactory *instance();

public:
    MImage::ptr decodeFFData(const MData::ptr &ffData);
    MImage::ptr decodeBitmap(const MData::ptr &bitmap, int width, int height);

    MData::ptr encodeFFData(const MImage::ptr &image, MImageFileFormat format);
    MData::ptr encodeBitmap(const MImage::ptr &image);

    MSize::ptr getPixelSize(const MImage::ptr &image);

protected:
    M_HOST_IMPL virtual
        MImage::ptr onDecodeFFData(const MData::ptr &ffData);
    
    M_HOST_IMPL virtual
        MImage::ptr onDecodeBitmap(const MData::ptr &bitmap, int width, int height);

    M_HOST_IMPL virtual
        MData::ptr onEncodeFFData(const MImage::ptr &image, MImageFileFormat format);
    
    M_HOST_IMPL virtual
        MData::ptr onEncodeBitmap(const MImage::ptr &image);

    M_HOST_IMPL virtual
        MSize::ptr onGetPixelSize(const MImage::ptr &image);

private:
    static MImageFactory::ptr sInstance;
};
