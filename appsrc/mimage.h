#pragma once

#include "mgeometry.h"
#include "mgraphics.h" //bitmap processing will use color struct.

//file format:

minikit_enum(MImageFileFormat) {
    JPEG = 1,
    PNG  = 2,
};

//image:

minikit_class(MVirtualImage, MObject) {
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

    MVirtualImage::ptr real();

private:
    MVirtualImage::ptr mReal;
};

//image factory:

minikit_class(MVirtualImageFactory, MObject) {
public:
    MINIKIT_HOST_CALL static void setInstance(const MVirtualImageFactory::ptr &obj);
    static MVirtualImageFactory *instance();

public:
    virtual MVirtualImage::ptr imageFromFFData(const MVector<uint8_t>::ptr &ffData);
    virtual MVirtualImage::ptr imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height);

    virtual MVector<uint8_t>::ptr ffDataFromImage(const MVirtualImage::ptr &real, MImageFileFormat format);
    virtual MVector<uint8_t>::ptr bitmapFromImage(const MVirtualImage::ptr &real);

    virtual MSize::ptr pixelSize(const MVirtualImage::ptr &real);

private:
    static MVirtualImageFactory::ptr sInstance;
};
