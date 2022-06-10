#pragma once

#include "mimage.h"
#include "mwin32gdiplus.h"

minikit_class(MWin32Image, MImage)
{
public:
    std::shared_ptr<Gdiplus::Image> mGdiImage;
};

minikit_class(MWin32ImageFactory, MImageFactory)
{
public:
    static void install();
    
protected:
    MImage::ptr onDecodeFFData(const MVector<uint8_t>::ptr &ffData) override;
    MImage::ptr onDecodeBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) override;

    MVector<uint8_t>::ptr onEncodeFFData(const MImage::ptr &real, MImageFileFormat format) override;
    MVector<uint8_t>::ptr onEncodeBitmap(const MImage::ptr &real) override;

    MSize::ptr onGetPixelSize(const MImage::ptr &real) override;
};
