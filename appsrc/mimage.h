#pragma once

#include "mgeometry.h"
#include "mgraphics.h" //bitmap processing will use color struct.

//file format:

m_enum(MImageFileFormat) {
    JPEG = 1,
    PNG  = 2,
};

//image:

m_class(MImageImpl, MObject) {
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

    MImageImpl::ptr impl();

private:
    MImageImpl::ptr mImpl;
};

//image factory:

m_class(MImageFactory, MObject) {
public:
    M_HOST_NEED_CALL static void setInstance(const MImageFactory::ptr &obj);
    static MImageFactory *instance();

public:
    //image from file format data.
    virtual MImageImpl::ptr imageFromFFData(const MVector<uint8_t>::ptr &ffData);
    //image from rgba 32-bit depth bitmap.
    virtual MImageImpl::ptr imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height);

    //file format data from image.
    virtual MVector<uint8_t>::ptr ffDataFromImage(const MImageImpl::ptr &impl, MImageFileFormat format);
    //rgba 32-bit depth bitmap from image.
    virtual MVector<uint8_t>::ptr bitmapFromImage(const MImageImpl::ptr &impl);

    //image pixel size.
    virtual MSize::ptr pixelSize(const MImageImpl::ptr &impl);

private:
    static MImageFactory::ptr sInstance;
};
