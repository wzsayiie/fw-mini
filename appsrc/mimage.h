#pragma once

#include "mgeometry.h"

//image:
//

M_HOST_IMPLEMENT_CLASS
declare_reflectable_class(MImageImpl)
class MImageImpl : public MExtends<MImageImpl, MObject> {
};

declare_reflectable_class(MImage)
class MImage : public MExtends<MImage, MObject> {
public:
    static MImage::ptr imageFromBundle(const std::string &path);
    static MImage::ptr imageFromFile  (const std::string &path);
    static MImage::ptr imageFromFFData(const MVector<uint8_t>::ptr &ffData);
    static MImage::ptr imageFromBitmap(const MVector<uint8_t>::ptr bitmap, int width, int height);

    void writeFile(const std::string &path);

    MVector<uint8_t>::ptr copyFFData();
    MVector<uint8_t>::ptr copyBitmap();

    MSize::ptr sizePixel();

private:
    MImageImpl::ptr sImpl;
};

//image factory:
//

M_HOST_IMPLEMENT_CLASS
declare_reflectable_class(MImageFactory)
class MImageFactory : public MExtends<MImageFactory, MObject> {
public:
    static void setSharedObject(const MImageFactory::ptr &factory);
    static MImageFactory *sharedObject();

public:
    //image from file format data.
    virtual MImageImpl::ptr imageFromFFData(const MVector<uint8_t>::ptr ffData);
    //image from 32-bit depth bitmap.
    virtual MImageImpl::ptr imageFromBitmap(const MVector<uint8_t>::ptr bitmap, int width, int height);

    //file format data from image.
    virtual MVector<uint8_t>::ptr ffDataFromImage(const MImageImpl::ptr image);
    //32-bit depth bitmap from image.
    virtual MVector<uint8_t>::ptr bitmapFromImage(const MImageImpl::ptr image);

    //image pixel size.
    virtual MSize::ptr sizePixel();

private:
    static MImageFactory::ptr sFactory;
};
