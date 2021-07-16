#include "mwinapi.h"
#include <map>
#include <shlobj.h>
#include "mhostapi.h"
#include "mtypeex.h"

template<typename T> class ManagedObjectPool
{
public:
    int add(T *object)
    {
        mPool[++mIdCount] = object;
        return mIdCount;
    }

    T *get(int id)
    {
        auto it = mPool.find(id);
        return it != mPool.end() ? it->second : nullptr;
    }

    void remove(int id)
    {
        auto it = mPool.find(id);
        if (it != mPool.end())
        {
            mPool.erase(it);
        }
    }

private:
    std::map<int, T *> mPool;
    int mIdCount = 0;
};

static ManagedObjectPool<Gdiplus::Image> *ManagedImagePool()
{
    static ManagedObjectPool<Gdiplus::Image> *pool = nullptr;
    if (!pool)
    {
        pool = new ManagedObjectPool<Gdiplus::Image>;
    }
    return pool;
}

Gdiplus::Image *MManagedImage(int id)
{
    return ManagedImagePool()->get(id);
}

static void PrintMessage(MString *text)
{
    wprintf(L"%s\n", (const WCHAR *)MStringU16Chars(text));
}

static MData *CopyBundleAsset(MString *path)
{
    return nullptr;
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

    int imageId = ManagedImagePool()->add(image);
    return MImageCreate(imageId, [](int id) { ManagedImagePool()->remove(id); });
}

static MString *CopyAppDirectoryWithParent(const WCHAR *parent)
{
    //join the directory path.
    std::basic_string<WCHAR> allPath = parent;
    if (*allPath.rbegin() != L'\\')
    {
        allPath.append(L"\\");
    }
    allPath.append((const WCHAR *)_MAppDirectoryU16Name);

    //if the directory don't exist, then create it.
    MString *thisPath = MStringCreateU16((const char16_t *)allPath.c_str());
    if (!MDirectoryExists(thisPath))
    {
        MMakeDirectory(thisPath);
    }
    return thisPath;
}

static MString *CopyDocumentPath()
{
    WCHAR document[MAX_PATH] = L"\0";
    SHGetFolderPathW(nullptr, CSIDL_PERSONAL, nullptr, 0, document);

    return CopyAppDirectoryWithParent(document);
}

static MString *CopyCachePath()
{
    WCHAR home[MAX_PATH] = L"\0";
    SHGetFolderPathW(nullptr, CSIDL_PROFILE, nullptr, 0, home);

    return CopyAppDirectoryWithParent(home);
}

static MString *CopyTemporaryPath()
{
    WCHAR temporary[MAX_PATH] = L"\0";
    GetTempPathW(MAX_PATH, temporary);

    return CopyAppDirectoryWithParent(temporary);
}

static bool MakeDirectory(MString *path)
{
    std::basic_string<WCHAR> dirPath = (const WCHAR *)MStringU16Chars(path);

    //SHCreateDirectoryEx() can create a continuous path like "xx\xx\xx",
    //but it's parameter does not support relative path.
    if (dirPath.size() >= 2 && dirPath[1] != L':')
    {
        WCHAR buffer[MAX_PATH] = {0};
        GetCurrentDirectoryW(MAX_PATH, buffer);

        std::basic_string<WCHAR> allPath = buffer;
        if (*allPath.rbegin() != L'\\')
        {
            allPath.append(L"\\");
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
    std::basic_string<WCHAR> target = (const WCHAR *)MStringU16Chars(path);
    if (*target.rbegin() != '\\')
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
    std::basic_string<WCHAR> from = (const WCHAR *)MStringU16Chars(path);
    from.push_back('\0');

    SHFILEOPSTRUCTW operation = {0};
    operation.wFunc  = FO_DELETE;
    operation.pFrom  = from.c_str();
    operation.pTo    = nullptr;
    operation.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;

    SHFileOperationW(&operation);
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
    _MSetApiPrintMessage     (PrintMessage     );
    _MSetApiCopyBundleAsset  (CopyBundleAsset  );
    _MSetApiCreateImage      (CreateImage      );
    _MSetApiCopyDocumentPath (CopyDocumentPath );
    _MSetApiCopyCachePath    (CopyCachePath    );
    _MSetApiCopyTemporaryPath(CopyTemporaryPath);
    _MSetApiMakeDirectory    (MakeDirectory    );
    _MSetApiCopyPathSubItems (CopyPathSubItems );
    _MSetApiRemovePath       (RemovePath       );
    _MSetApiPathExists       (PathExists       );
    _MSetApiDirectoryExists  (DirectoryExists  );
    _MSetApiFileExists       (FileExists       );
}
