#pragma once

#include "mimage.h"
#include "mwin32gdiplus.h"

m_class(MWin32Image, MImage)
{
public:
    std::shared_ptr<Gdiplus::Image> mGdiImage;
};

m_class(MWin32ImageFactory, MImageFactory)
{
public:
    static void install();
    
protected:
    MImage::ptr onDecodeFFData(const MData::ptr &ffData) override;
    MImage::ptr onDecodeBitmap(const MData::ptr &bitmap, int width, int height) override;

    MData::ptr onEncodeFFData(const MImage::ptr &real, MImageFileFormat format) override;
    MData::ptr onEncodeBitmap(const MImage::ptr &real) override;

    MSize::ptr onGetPixelSize(const MImage::ptr &real) override;
};
