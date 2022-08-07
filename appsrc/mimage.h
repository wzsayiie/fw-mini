#pragma once

#include "mcontainers.h"
#include "mgeometry.h"
#include "mgraphics.h" //bitmap processing will use color struct.

//image:

m_enum(MImageFileFormat) {
    JPEG = 1,
    PNG  = 2,
};

m_class(MImage, MObject) {
public:
    static MImage::ptr fromBundle(const std::string &path);
    static MImage::ptr fromFile  (const std::string &path);
    static MImage::ptr fromFFData(const MVector<uint8_t>::ptr &ffData);
    static MImage::ptr fromBitmap(const MVector<uint8_t>::ptr bitmap, int width, int height);

public:
    void writeFile(const std::string &path, MImageFileFormat format);

    MVector<uint8_t>::ptr copyFFData(MImageFileFormat format);
    MVector<uint8_t>::ptr copyBitmap();

    MSize::ptr pixelSize();
};

//image factory:

m_class(MImageFactory, MObject) {
public:
    M_HOST_CALL static void setInstance(const MImageFactory::ptr &obj);
    static MImageFactory *instance();

public:
    MImage::ptr decodeFFData(const MVector<uint8_t>::ptr &ffData);
    MImage::ptr decodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height);

    MVector<uint8_t>::ptr encodeFFData(const MImage::ptr &image, MImageFileFormat format);
    MVector<uint8_t>::ptr encodeBitmap(const MImage::ptr &image);

    MSize::ptr getPixelSize(const MImage::ptr &image);

protected:
    M_HOST_IMPL virtual
        MImage::ptr onDecodeFFData(const MVector<uint8_t>::ptr &ffData);
    
    M_HOST_IMPL virtual
        MImage::ptr onDecodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height);

    M_HOST_IMPL virtual
        MVector<uint8_t>::ptr onEncodeFFData(const MImage::ptr &image, MImageFileFormat format);
    
    M_HOST_IMPL virtual
        MVector<uint8_t>::ptr onEncodeBitmap(const MImage::ptr &image);

    M_HOST_IMPL virtual
        MSize::ptr onGetPixelSize(const MImage::ptr &image);

private:
    static MImageFactory::ptr sInstance;
};
