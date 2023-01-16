#pragma once

#include "mdata.h"
#include "mgeometry.h"

//image:

m_enum(MImageFileFormat) {
    JPEG = 1,
    PNG  = 2,
};

m_class(MImage, MObject) {
public:
    static MImage::ptr fromBundle(const std::string &path  );
    static MImage::ptr fromFile  (const std::string &path  );
    static MImage::ptr fromFFData(const MData::ptr  &ffData);
    static MImage::ptr fromBitmap(const MData::ptr  &bitmap, int width, int height);

public:
    void writeFile(const std::string &path, MImageFileFormat format);

    MData::ptr copyFFData(MImageFileFormat format);
    MData::ptr copyBitmap();

    MSize::ptr pixelSize();
};

//image factory:

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
