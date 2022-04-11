#include "mwin32imagefactory.h"

void MWin32ImageFactory::install()
{
    auto obj = MWin32ImageFactory::create();
    setSharedObject(obj);
}

MImageImpl::ptr MWin32ImageFactory::imageFromFFData(const MVector<uint8_t>::ptr &ffData)
{
    return nullptr;
}

MImageImpl::ptr MWin32ImageFactory::imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height)
{
    return nullptr;
}

MVector<uint8_t>::ptr MWin32ImageFactory::ffDataFromImage(const MImageImpl::ptr &impl, MImageFileFormat format)
{
    return nullptr;
}

MVector<uint8_t>::ptr MWin32ImageFactory::bitmapFromImage(const MImageImpl::ptr &impl)
{
    return nullptr;
}

MSize::ptr MWin32ImageFactory::pixelSize(const MImageImpl::ptr &impl)
{
    return nullptr;
}
