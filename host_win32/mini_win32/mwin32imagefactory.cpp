#include "mwin32imagefactory.h"

void MWin32ImageFactory::install()
{
    auto obj = MWin32ImageFactory::create();
    setInstance(obj);
}

MVirtualImage::ptr MWin32ImageFactory::imageFromFFData(const MVector<uint8_t>::ptr &ffData)
{
    //copy the memory data:
    HGLOBAL memory = GlobalAlloc(GMEM_FIXED, (SIZE_T)ffData->size());
    dash_defer { GlobalFree(memory); };

    if (void *bytes = GlobalLock(memory))
    {
        memcpy(bytes, ffData->data(), ffData->size());
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
    Gdiplus::Image *gdiImage = Gdiplus::Image::FromStream(stream, FALSE);
    if (gdiImage)
    {
        auto real = MWin32Image::create();
        real->mGdiImage = std::shared_ptr<Gdiplus::Image>(gdiImage);
        return real;
    }
    return nullptr;
}

MVirtualImage::ptr MWin32ImageFactory::imageFromBitmap(const MVector<uint8_t>::ptr &bitmap, int width, int height)
{
    auto gdiImage = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
    auto pixels   = (MColorRGBA *)bitmap->data();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            MColorRGBA *src = pixels + width * y + x;
            Gdiplus::Color dst(src->alpha, src->red, src->green, src->blue);

            gdiImage->SetPixel(x, y, dst);
        }
    }

    auto real = MWin32Image::create();
    real->mGdiImage = std::shared_ptr<Gdiplus::Image>(gdiImage);
    return real;
}

MVector<uint8_t>::ptr MWin32ImageFactory::ffDataFromImage(const MVirtualImage::ptr &real, MImageFileFormat format)
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

    Gdiplus::Image *gdiImage = std::static_pointer_cast<MWin32Image>(real)->mGdiImage.get();
    gdiImage->Save(stream, &encoder);

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
    ffData->resize(size);

    if (void *bytes = GlobalLock(memory))
    {
        memcpy(ffData->data(), bytes, size);
        GlobalUnlock(bytes);

        return ffData;
    }
    else
    {
        return nullptr;
    }
}

MVector<uint8_t>::ptr MWin32ImageFactory::bitmapFromImage(const MVirtualImage::ptr &real)
{
    //draw the image on a bitmap context:
    Gdiplus::Image *gdiImage = std::static_pointer_cast<MWin32Image>(real)->mGdiImage.get();
    auto width  = gdiImage->GetWidth ();
    auto height = gdiImage->GetHeight();

    std::shared_ptr<Gdiplus::Bitmap  > bitmap  (new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB));
    std::shared_ptr<Gdiplus::Graphics> graphics(Gdiplus::Graphics::FromImage(bitmap.get()));
    graphics->DrawImage(gdiImage, 0, 0, width, height);

    //copy bitmap bytes:
    auto bytes = MVector<uint8_t>::create();
    bytes->resize((size_t)(width * height * 4));

    auto pixels = (MColorRGBA *)bytes->data();
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

MSize::ptr MWin32ImageFactory::pixelSize(const MVirtualImage::ptr &real)
{
    Gdiplus::Image *gdiImage = std::static_pointer_cast<MWin32Image>(real)->mGdiImage.get();

    auto width  = (float)gdiImage->GetWidth ();
    auto height = (float)gdiImage->GetHeight();
    return MSize::from(width, height);
}
