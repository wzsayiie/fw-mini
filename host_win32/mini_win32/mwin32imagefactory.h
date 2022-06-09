#pragma once

#include "mimage.h"
#include "mwin32gdiplus.h"

minikit_class(MWin32Image, MVirtualImage) {
public:
    std::shared_ptr<Gdiplus::Image> mGdiImage;
};

minikit_class(MWin32ImageFactory, MVirtualImageFactory) {
public:
    static void install();
    
public:
    MVirtualImage::ptr imageFromFFData(const MVector<uint8_t>::ptr &ffData) override;
    MVirtualImage::ptr imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) override;

    MVector<uint8_t>::ptr ffDataFromImage(const MVirtualImage::ptr &real, MImageFileFormat format) override;
    MVector<uint8_t>::ptr bitmapFromImage(const MVirtualImage::ptr &real) override;

    MSize::ptr pixelSize(const MVirtualImage::ptr &real) override;
};
