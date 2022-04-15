#include "mwin32imagefactory.h"

void MWin32ImageFactory::install()
{
    auto obj = MWin32ImageFactory::create();
    setSharedObject(obj);
}

MImageImpl::ptr MWin32ImageFactory::imageFromFFData(const MVector<uint8_t>::ptr &ffData)
{
    //copy the memory data:
    HGLOBAL memory = GlobalAlloc(GMEM_FIXED, (SIZE_T)ffData->vector.size());
    dash_defer { GlobalFree(memory); };

    if (void *bytes = GlobalLock(memory))
    {
        memcpy(bytes, ffData->vector.data(), ffData->vector.size());
        GlobalUnlock(memory);
    }
    else
    {
        return nullptr;
    }

    //create a stream object:
    IStream *stream = nullptr;
    HRESULT streamResult = CreateStreamOnHGlobal(memory, FALSE, &stream);
    if (streamResult != S_OK)
    {
        return nullptr;
    }

    dash_defer { stream->Release(); };

    //create the image.
    Gdiplus::Image *real = Gdiplus::Image::FromStream(stream, FALSE);
    if (real)
    {
        auto impl = MWin32ImageImpl::create();
        impl->mReal = std::shared_ptr<Gdiplus::Image>(real);
        return impl;
    }
    return nullptr;
}

MImageImpl::ptr MWin32ImageFactory::imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height)
{
    auto real   = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
    auto pixels = (MColorRGBA *)bitmap->vector.data();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            MColorRGBA *src = pixels + width * y + x;
            Gdiplus::Color dst(src->alpha, src->red, src->green, src->blue);

            real->SetPixel(x, y, dst);
        }
    }

    auto impl = MWin32ImageImpl::create();
    impl->mReal = std::shared_ptr<Gdiplus::Image>(real);
    return impl;
}

MVector<uint8_t>::ptr MWin32ImageFactory::ffDataFromImage(const MImageImpl::ptr &impl, MImageFileFormat format)
{
    //find the encoder:
    const WCHAR *encoderId = nullptr;
    switch (format)
    {
    case MImageFileFormat::JPEG: encoderId = L"{557cf401-1a04-11d3-9a73-0000f81ef32e}"; break;
    case MImageFileFormat::PNG : encoderId = L"{557cf406-1a04-11d3-9a73-0000f81ef32e}"; break;
    default: return nullptr;
    }

    CLSID encoder = {0};
    HRESULT encoderResult = CLSIDFromString(encoderId, &encoder);
    if (encoderResult != S_OK)
    {
        return nullptr;
    }

    //write the image data to a stream:
    IStream *stream = nullptr;
    HRESULT streamResult = CreateStreamOnHGlobal(nullptr, TRUE, &stream);
    if (streamResult != S_OK)
    {
        return nullptr;
    }

    dash_defer { stream->Release(); };

    Gdiplus::Image *real = std::static_pointer_cast<MWin32ImageImpl>(impl)->mReal.get();
    real->Save(stream, &encoder);

    //get the data:
    HGLOBAL memory = nullptr;
    HRESULT memoryResult = GetHGlobalFromStream(stream, &memory);
    if (memoryResult != S_OK)
    {
        return nullptr;
    }

    SIZE_T size = GlobalSize(memory);

    //copy the data:
    auto ffData = MVector<uint8_t>::create();
    ffData->vector.resize(size);

    if (void *bytes = GlobalLock(memory))
    {
        memcpy(ffData->vector.data(), bytes, size);
        GlobalUnlock(bytes);

        return ffData;
    }
    else
    {
        return nullptr;
    }
}

MVector<uint8_t>::ptr MWin32ImageFactory::bitmapFromImage(const MImageImpl::ptr &impl)
{
    //draw the image on a bitmap context:
    Gdiplus::Image *real = std::static_pointer_cast<MWin32ImageImpl>(impl)->mReal.get();
    auto width  = real->GetWidth ();
    auto height = real->GetHeight();

    std::shared_ptr<Gdiplus::Bitmap  > bitmap  (new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB));
    std::shared_ptr<Gdiplus::Graphics> graphics(Gdiplus::Graphics::FromImage(bitmap.get()));
    graphics->DrawImage(real, 0, 0, width, height);

    //copy bitmap bytes:
    auto bytes = MVector<uint8_t>::create();
    bytes->vector.resize((size_t)(width * height * 4));

    auto pixels = (MColorRGBA *)bytes->vector.data();
    for (UINT x = 0; x < width; ++x)
    {
        for (UINT y = 0; y < height; ++y)
        {
            Gdiplus::Color src;
            bitmap->GetPixel(x, y, &src);

            MColorRGBA *dst = pixels + width * y + x;
            dst->red   = src.GetRed  ();
            dst->green = src.GetGreen();
            dst->blue  = src.GetBlue ();
            dst->alpha = src.GetAlpha();
        }
    }

    return bytes;
}

MSize::ptr MWin32ImageFactory::pixelSize(const MImageImpl::ptr &impl)
{
    Gdiplus::Image *real = std::static_pointer_cast<MWin32ImageImpl>(impl)->mReal.get();

    auto width  = (float)real->GetWidth ();
    auto height = (float)real->GetHeight();
    return MSize::from(width, height);
}
