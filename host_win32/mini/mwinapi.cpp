#define _CRT_SECURE_NO_WARNINGS

#include "mwinapi.h"
#include <shlobj.h>

//native types:

MWin32Image::MWin32Image(Gdiplus::Image *gdiImage)
{
    mGdiImage = gdiImage;
}

MWin32Image::~MWin32Image()
{
    delete mGdiImage;
}

Gdiplus::Image *MWin32Image::gdiImage()
{
    return mGdiImage;
}

//api assist:

static MString *CopyAppDirectory(const WCHAR *parent, const WCHAR *directory)
{
    //join the directory path.
    std::wstring allPath = parent;
    if (allPath.back() != L'\\')
    {
        allPath.push_back(L'\\');
    }
    allPath.append(directory);

    //if the directory don't exist, then create it.
    MString *thisPath = MStringCreateU16((const char16_t *)allPath.c_str());
    if (!MDirectoryExists(thisPath))
    {
        MMakeDirectory(thisPath);
    }
    return thisPath;
}

static bool PathExistsAt(MString *path, bool *isDir)
{
    auto  thisPath   = (const WCHAR *)MStringU16Chars(path);
    DWORD attributes = GetFileAttributesW(thisPath);

    if (attributes != INVALID_FILE_ATTRIBUTES)
    {
        *isDir = attributes & FILE_ATTRIBUTE_DIRECTORY;
        return true;
    }
    else
    {
        return false;
    }
}

//host api:

static void PrintMessage(MString *text)
{
    wprintf(L"%s\n", (const WCHAR *)MStringU16Chars(text));
}

const int ReadAssetBufferSize = 8 * 1024;

static MData *CopyBundleAsset(MString *path)
{
    //join the asset path.
    static std::wstring *bundlePath = nullptr;
    if (!bundlePath)
    {
        WCHAR buffer[MAX_PATH] = {0};
        GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        
        WCHAR *base = nullptr;
        //in development mode, the bundle is in the project directory.
        if (base = wcsstr(buffer, L"fw-mini\\"))
        {
            base[8] = L'\0';
            wcscat(buffer, L"appres\\");
        }
        //in the run mode, the bundle and the program are in the same directory.
        else if (base = wcsrchr(buffer, L'\\'))
        {
            base[1] = L'\0';
        }

        bundlePath = new std::wstring;
        if (base)
        {
            wcscat(buffer, (const WCHAR *)MAssetBundleU16Name);
            wcscat(buffer, L"\\");
            bundlePath->append(buffer);
        }
    }

    std::wstring assetPath;
    assetPath.append(*bundlePath);
    assetPath.append((const WCHAR *)MStringU16Chars(path));

    //read the asset file.
    FILE *assetFile = _wfopen(assetPath.c_str(), L"rb");
    if (!assetFile)
    {
        return nullptr;
    }

    MData *assetData = MDataCreate(nullptr, 0);
    uint8_t buffer[ReadAssetBufferSize];
    while (!feof(assetFile))
    {
        auto size = (int)fread(buffer, 1, ReadAssetBufferSize, assetFile);
        MDataAppend(assetData, buffer, size);
    }
    fclose(assetFile);
    return assetData;
}

static MImage *CreateImage(MData *data)
{
    //create the memory block.
    HGLOBAL memory = GlobalAlloc(GMEM_FIXED, MDataSize(data));
    if (!memory)
    {
        return nullptr;
    }
    void *memoryBytes = GlobalLock(memory);
    if (memoryBytes)
    {
        memcpy(memoryBytes, MDataBytes(data), MDataSize(data));
    }
    GlobalUnlock(memory);

    //create the stream.
    IStream *stream = nullptr;
    HRESULT streamResult = CreateStreamOnHGlobal(memory, TRUE /* auto free "memory" */, &stream);
    if (streamResult != S_OK)
    {
        GlobalFree(memory);
        return nullptr;
    }

    //create the image.
    Gdiplus::Image *image = Gdiplus::Image::FromStream(stream, FALSE);
    stream->Release();

    if (image)
    {
        return new MWin32Image(image);
    }
    return nullptr;
}

static MImage *CreateBitmapImage(MData *data, int width, int height)
{
    auto bitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
    if (!bitmap)
    {
        return nullptr;
    }

    auto colors = (MColorPattern *)MDataBytes(data);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            MColorPattern *src = colors + y * width + x; 
            Gdiplus::Color dst(src->alpha, src->red, src->green, src->blue);

            bitmap->SetPixel(x, y, dst);
        }
    }

    return new MWin32Image(bitmap);
}

static MData *CopyImageBitmap(MImage *image)
{
    return nullptr;
}

static int ImagePixelWidth (MImage *i) { return ((MWin32Image *)i)->gdiImage()->GetWidth (); }
static int ImagePixelHeight(MImage *i) { return ((MWin32Image *)i)->gdiImage()->GetHeight(); }

static MString *CopyDocumentPath()
{
    WCHAR document[MAX_PATH] = L"\0";
    SHGetFolderPathW(nullptr, CSIDL_PERSONAL, nullptr, 0, document);

    return CopyAppDirectory(document, (const WCHAR *)MPrivateDirectoryU16Name);
}

static MString *CopyCachePath()
{
    WCHAR home[MAX_PATH] = L"\0";
    SHGetFolderPathW(nullptr, CSIDL_PROFILE, nullptr, 0, home);

    return CopyAppDirectory(home, (const WCHAR *)MPrivateDirectoryU16Name);
}

static MString *CopyTemporaryPath()
{
    WCHAR temporary[MAX_PATH] = L"\0";
    GetTempPathW(MAX_PATH, temporary);

    return CopyAppDirectory(temporary, (const WCHAR *)MPrivateDirectoryU16Name);
}

static bool MakeDirectory(MString *path)
{
    std::wstring dirPath = (const WCHAR *)MStringU16Chars(path);

    //SHCreateDirectoryEx() can create a continuous path like "xx\xx\xx",
    //but it's parameter does not support relative path.
    if (dirPath.size() >= 2 && dirPath[1] != L':')
    {
        WCHAR buffer[MAX_PATH] = {0};
        GetCurrentDirectoryW(MAX_PATH, buffer);

        std::wstring allPath = buffer;
        if (allPath.back() != L'\\')
        {
            allPath.push_back(L'\\');
        }
        allPath.append(dirPath);

        int errorCode = SHCreateDirectoryExW(nullptr, allPath.c_str(), nullptr);
        return errorCode == ERROR_SUCCESS;
    }
    else
    {
        int errorCode = SHCreateDirectoryExW(nullptr, dirPath.c_str(), nullptr);
        return errorCode == ERROR_SUCCESS;
    }
}

static MArray *CopyPathSubItems(MString *path)
{
    //construct the pattern string "xx\*".
    std::wstring target = (const WCHAR *)MStringU16Chars(path);
    if (target.back() != '\\')
    {
        target.append(L"\\*");
    }
    else
    {
        target.append(L"*");
    }

    //traverse the subitems.
    WIN32_FIND_DATAW data = {0};
    HANDLE state = FindFirstFileW(target.c_str(), &data);
    if (state == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }

    MArray *array = MArrayCreate();
    while (true)
    {
        do
        {
            //ignore hidden files.
            if (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
            {
                break;
            }

            //ignore "." and "..".
            const WCHAR *name = data.cFileName;
            if (wcscmp(name, L".") == 0 || wcscmp(name, L"..") == 0)
            {
                break;
            }

            MStringRef item = m_auto_release MStringCreateU16((const char16_t *)name);
            MArrayAppend(array, item.get());
        }
        while (false);

        if (!FindNextFileW(state, &data))
        {
            break;
        }
    }
    FindClose(state);

    return array;
}

static void RemovePath(MString *path)
{
    //SHFILEOPSTRUCT.pFrom need double null terminating.
    std::wstring from = (const WCHAR *)MStringU16Chars(path);
    from.push_back('\0');

    SHFILEOPSTRUCTW operation = {0};
    operation.wFunc  = FO_DELETE;
    operation.pFrom  = from.c_str();
    operation.pTo    = nullptr;
    operation.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;

    SHFileOperationW(&operation);
}

static bool PathExists(MString *path)
{
    bool isDir = false;
    return PathExistsAt(path, &isDir);
}

static bool DirectoryExists(MString *path)
{
    bool isDir = false;
    bool exist = PathExistsAt(path, &isDir);
    return exist && isDir;
}

static bool FileExists(MString *path)
{
    bool isDir = false;
    bool exist = PathExistsAt(path, &isDir);
    return exist && !isDir;
}

void MRegisterApi()
{
    _MSetApi_PrintMessage     (PrintMessage     );
    _MSetApi_CopyBundleAsset  (CopyBundleAsset  );
    _MSetApi_CreateImage      (CreateImage      );
    _MSetApi_CreateBitmapImage(CreateBitmapImage);
    _MSetApi_CopyImageBitmap  (CopyImageBitmap  );
    _MSetApi_ImagePixelWidth  (ImagePixelWidth  );
    _MSetApi_ImagePixelHeight (ImagePixelHeight );
    _MSetApi_CopyDocumentPath (CopyDocumentPath );
    _MSetApi_CopyCachePath    (CopyCachePath    );
    _MSetApi_CopyTemporaryPath(CopyTemporaryPath);
    _MSetApi_MakeDirectory    (MakeDirectory    );
    _MSetApi_CopyPathSubItems (CopyPathSubItems );
    _MSetApi_RemovePath       (RemovePath       );
    _MSetApi_PathExists       (PathExists       );
    _MSetApi_DirectoryExists  (DirectoryExists  );
    _MSetApi_FileExists       (FileExists       );
}
