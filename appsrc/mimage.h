#pragma once

#include "mgeometry.h"
#include "mgraphics.h" //bitmap processing will use color struct.

//image:

minikit_enum(MImageFileFormat) {
    JPEG = 1,
    PNG  = 2,
};

minikit_class(MImage, MObject) {
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

minikit_class(MImageFactory, MObject) {
public:
    MINIKIT_HOST_CALL static void setInstance(const MImageFactory::ptr &obj);
    static MImageFactory *instance();

public:
    MImage::ptr decodeFFData(const MVector<uint8_t>::ptr &ffData);
    MImage::ptr decodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height);

    MVector<uint8_t>::ptr encodeFFData(const MImage::ptr &image, MImageFileFormat format);
    MVector<uint8_t>::ptr encodeBitmap(const MImage::ptr &image);

    MSize::ptr getPixelSize(const MImage::ptr &image);

protected:
    MINIKIT_HOST_IMPL virtual
        MImage::ptr onDecodeFFData(const MVector<uint8_t>::ptr &ffData);
    
    MINIKIT_HOST_IMPL virtual
        MImage::ptr onDecodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height);

    MINIKIT_HOST_IMPL virtual
        MVector<uint8_t>::ptr onEncodeFFData(const MImage::ptr &image, MImageFileFormat format);
    
    MINIKIT_HOST_IMPL virtual
        MVector<uint8_t>::ptr onEncodeBitmap(const MImage::ptr &image);

    MINIKIT_HOST_IMPL virtual MSize::ptr onGetPixelSize(const MImage::ptr &image);

private:
    static MImageFactory::ptr sInstance;
};
