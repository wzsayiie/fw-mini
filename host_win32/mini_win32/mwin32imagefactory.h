#pragma once

#include "mimage.h"
#include "mwin32gdiplus.h"

m_class(MWin32ImageImpl, MImageImpl) {
public:
    std::shared_ptr<Gdiplus::Image> mReal;
};

m_class(MWin32ImageFactory, MImageFactory) {
public:
    static void install();
    
public:
    MImageImpl::ptr imageFromFFData(const MVector<uint8_t>::ptr &ffData) override;
    MImageImpl::ptr imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height) override;

    MVector<uint8_t>::ptr ffDataFromImage(const MImageImpl::ptr &impl, MImageFileFormat format) override;
    MVector<uint8_t>::ptr bitmapFromImage(const MImageImpl::ptr &impl) override;

    MSize::ptr pixelSize(const MImageImpl::ptr &impl) override;
};
