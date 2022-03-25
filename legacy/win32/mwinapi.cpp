// #define _CRT_SECURE_NO_WARNINGS

// #include "mwinapi.h"
// #include <filesystem>
// #include <shlobj.h>

// //native types:

// MWin32Image::MWin32Image(Gdiplus::Image *gdiImage)
// {
//     mGdiImage = gdiImage;
// }

// MWin32Image::~MWin32Image()
// {
//     delete mGdiImage;
// }

// Gdiplus::Image *MWin32Image::gdiImage()
// {
//     return mGdiImage;
// }

// //api assist:

// static MString *CreateAppDirectory(const WCHAR *parent, const WCHAR *directory)
// {
//     std::filesystem::path path(parent);
//     path.append(directory);

//     std::filesystem::create_directories(path);

//     return MStringCreateU16((const char16_t *)path.c_str());
// }

// //host api:

// static void PrintMessage(MString *text)
// {
//     wprintf(L"%s\n", (const WCHAR *)MStringU16Chars(text));
// }

// static MData *CopyBundleAsset(MString *path)
// {
//     //join the asset path:
//     static std::filesystem::path *bundlePath = nullptr;
//     if (!bundlePath)
//     {
//         WCHAR buffer[MAX_PATH] = {0};
//         GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        
//         if (WCHAR *ptr = wcsstr(buffer, L"fw-mini"))
//         {
//             //in development mode, the bundle is in the project directory.
//             ptr[0] = L'\0';
//             wcscat(buffer, L"fw-mini\\appres");
//         }
//         else if (WCHAR *ptr = wcsrchr(buffer, L'\\'))
//         {
//             //in the run mode, the bundle and the program are in the same directory.
//             ptr[0] = L'\0';
//         }

//         bundlePath = new std::filesystem::path(buffer);
//         bundlePath->append(MAssetBundleU16Name);
//     }

//     std::filesystem::path assetPath(*bundlePath);
//     assetPath.append(MStringU16Chars(path));

//     //read the asset file:
//     FILE *assetFile = _wfopen(assetPath.c_str(), L"rb");
//     if (!assetFile)
//     {
//         return nullptr;
//     }

//     MData *assetData = MDataCreate(0);
//     uint8_t buffer[1024 * 8];
//     while (!feof(assetFile))
//     {
//         auto size = (int)fread(buffer, 1, sizeof(buffer), assetFile);
//         MDataAppend(assetData, buffer, size);
//     }
//     fclose(assetFile);
//     return assetData;
// }

// static MImage *CreateImage(MData *data)
// {
//     //create the memory block.
//     HGLOBAL memory = GlobalAlloc(GMEM_FIXED, MDataSize(data));
//     if (!memory)
//     {
//         return nullptr;
//     }
//     void *memoryBytes = GlobalLock(memory);
//     if (memoryBytes)
//     {
//         memcpy(memoryBytes, MDataBytes(data), MDataSize(data));
//     }
//     GlobalUnlock(memory);

//     //create the stream.
//     IStream *stream = nullptr;
//     HRESULT streamResult = CreateStreamOnHGlobal(memory, TRUE /* auto free "memory" */, &stream);
//     if (streamResult != S_OK)
//     {
//         GlobalFree(memory);
//         return nullptr;
//     }

//     //create the image.
//     Gdiplus::Image *image = Gdiplus::Image::FromStream(stream, FALSE);
//     stream->Release();

//     if (image)
//     {
//         return new MWin32Image(image);
//     }
//     return nullptr;
// }

// static MImage *CreateBitmapImage(MData *data, int width, int height)
// {
//     auto bitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
//     auto colors = (MColorPattern *)MDataBytes(data);

//     for (int y = 0; y < height; ++y)
//     {
//         for (int x = 0; x < width; ++x)
//         {
//             MColorPattern *src = colors + width * y + x; 
//             Gdiplus::Color dst(src->alpha, src->red, src->green, src->blue);

//             bitmap->SetPixel(x, y, dst);
//         }
//     }

//     return new MWin32Image(bitmap);
// }

// static MData *CopyImageBitmap(MImage *image)
// {
//     Gdiplus::Image *gdiImage = ((MWin32Image *)image)->gdiImage();
//     int w = gdiImage->GetWidth ();
//     int h = gdiImage->GetHeight();

//     std::shared_ptr<Gdiplus::Bitmap  > bitmap  (new Gdiplus::Bitmap(w, h, PixelFormat32bppARGB));
//     std::shared_ptr<Gdiplus::Graphics> graphics(Gdiplus::Graphics::FromImage(bitmap.get()));
//     graphics->DrawImage(gdiImage, 0, 0, w, h);

//     MData *data = MDataCreate(w * h * 4);
//     auto colors = (MColorPattern *)MDataBytes(data);
//     for (int x = 0; x < w; ++x)
//     {
//         for (int y = 0; y < h; ++y)
//         {
//             Gdiplus::Color src;
//             bitmap->GetPixel(x, y, &src);

//             MColorPattern *dst = colors + w * y + x;
//             dst->red   = src.GetRed  ();
//             dst->green = src.GetGreen();
//             dst->blue  = src.GetBlue ();
//             dst->alpha = src.GetAlpha();
//         }
//     }
//     return data;
// }

// static int ImagePixelWidth (MImage *i) { return ((MWin32Image *)i)->gdiImage()->GetWidth (); }
// static int ImagePixelHeight(MImage *i) { return ((MWin32Image *)i)->gdiImage()->GetHeight(); }

// static MString *CopyDocumentPath()
// {
//     WCHAR document[MAX_PATH] = L"\0";
//     SHGetFolderPathW(nullptr, CSIDL_PERSONAL, nullptr, 0, document);

//     return CreateAppDirectory(document, (const WCHAR *)MPrivateDirectoryU16Name);
// }

// static MString *CopyCachePath()
// {
//     WCHAR home[MAX_PATH] = L"\0";
//     SHGetFolderPathW(nullptr, CSIDL_PROFILE, nullptr, 0, home);

//     return CreateAppDirectory(home, (const WCHAR *)MPrivateDirectoryU16Name);
// }

// static MString *CopyTemporaryPath()
// {
//     WCHAR temporary[MAX_PATH] = L"\0";
//     GetTempPathW(MAX_PATH, temporary);

//     return CreateAppDirectory(temporary, (const WCHAR *)MPrivateDirectoryU16Name);
// }

// void MRegisterApi()
// {
//     _MSetApi_PrintMessage     (PrintMessage     );
//     _MSetApi_CopyBundleAsset  (CopyBundleAsset  );
//     _MSetApi_CreateImage      (CreateImage      );
//     _MSetApi_CreateBitmapImage(CreateBitmapImage);
//     _MSetApi_CopyImageBitmap  (CopyImageBitmap  );
//     _MSetApi_ImagePixelWidth  (ImagePixelWidth  );
//     _MSetApi_ImagePixelHeight (ImagePixelHeight );
//     _MSetApi_CopyDocumentPath (CopyDocumentPath );
//     _MSetApi_CopyCachePath    (CopyCachePath    );
//     _MSetApi_CopyTemporaryPath(CopyTemporaryPath);
// }
