#include "cfilemanager.h"
#include "mhostapi.h"
#include "mresource.h"

//------------------------------------------------------------------------------
//path join:

inline bool IsPathSeperator(char chr) {
    #if MOS_WIN32
        return chr == '/' || chr == '\\';
    #else
        return chr == '/';
    #endif
}

#if MOS_WIN32
    const char PathSeperator = '\\';
#else
    const char PathSeperator = '/';
#endif

void CAppendPathItem(std::string *path, const std::string &item) {
    if (!path || item.empty()) {
        return;
    }

    if (!IsPathSeperator(path->back())) {
        path->push_back(PathSeperator);
    }
    path->append(item);
}

static int PenultimateSeperator(const std::string &path) {
    if (path.empty()) {
        return -1;
    }

    //skip seperators at the end.
    int it = (int)path.size() - 1;
    while (it >= 0 && IsPathSeperator(path[it])) {
        it -= 1;
    }

    //find the penultimate seperator.
    while (it >= 0 && !IsPathSeperator(path[it])) {
        it -= 1;
    }
    return it;
}

std::string CGetPathFilename(const std::string &path) {
    int seperator = PenultimateSeperator(path);
    if (seperator != -1) {
        auto begin = (size_t)seperator + 1;
        return path.substr(begin, (int)path.size() - begin);
    }
    return path;
}

std::string CGetPathParent(const std::string &path) {
    int seperator = PenultimateSeperator(path);
    if (seperator == -1) {
        return "";
    }
    
    auto size = (size_t)seperator;
    if (size == 0) {
        //the seperator at position [0].
        size = 1;
    }
    return path.substr(0, size);
}

//------------------------------------------------------------------------------
//file operations:

std::vector<uint8_t> CReadDataFromFile(const std::string &path) {
    MStringRef filePath = m_auto_release MStringCreateU8(path.c_str());
    MDataRef   fileData = m_auto_release MCopyDataFromFile(filePath.get());

    const uint8_t *bytes = MDataBytes(fileData.get());
    int size = MDataSize(fileData.get());
    return std::vector<uint8_t>(bytes, bytes + size);
}

std::string CReadStringFromFile(const std::string &path) {
    MStringRef filePath = m_auto_release MStringCreateU8(path.c_str());
    MStringRef string   = m_auto_release MCopyStringFromFile(filePath.get());

    if (string) {
        return std::string(MStringU8Chars(string.get()));
    }
    return "";
}

std::vector<uint8_t> CReadDataFromBundle(const std::string &path) {
    MStringRef assetPath = m_auto_release MStringCreateU8(path.c_str());
    MDataRef   assetData = m_auto_release MCopyBundleAsset(assetPath.get());

    const uint8_t *bytes = MDataBytes(assetData.get());
    int size = MDataSize(assetData.get());
    return std::vector<uint8_t>(bytes, bytes + size);
}

std::string CReadStringFromBundle(const std::string &path) {
    MStringRef filePath = m_auto_release MStringCreateU8(path.c_str());
    MStringRef string   = m_auto_release MCopyStringFromBundle(filePath.get());

    if (string) {
        return std::string(MStringU8Chars(string.get()));
    }
    return "";
}

void CWriteDataToFile(const std::string &path, const std::vector<uint8_t> &data) {
    MStringRef filePath = m_auto_release MStringCreateU8(path.c_str());
    MDataRef   fileData = m_auto_release MDataCreate(&data[0], (int)data.size());

    MWriteDataToFile(filePath.get(), fileData.get());
}

void CWriteU8StringToFile(const std::string &path, const std::string &data) {
    MStringRef filePath = m_auto_release MStringCreateU8(path.c_str());
    MStringRef string   = m_auto_release MStringCreateU8(data.c_str());

    MWriteU8StringToFile(filePath.get(), string.get());
}

void CWriteU16StringToFile(const std::string &path, const std::string &data) {
    MStringRef filePath = m_auto_release MStringCreateU8(path.c_str());
    MStringRef string   = m_auto_release MStringCreateU8(data.c_str());

    MWriteU16StringToFile(filePath.get(), string.get());
}

std::string CDocumentPath() {
    MStringRef path = m_auto_release MCopyDocumentPath();
    return MStringU8Chars(path.get());
}

std::string CCachePath() {
    MStringRef path = m_auto_release MCopyCachePath();
    return MStringU8Chars(path.get());
}

std::string CTemporaryPath() {
    MStringRef path = m_auto_release MCopyTemporaryPath();
    return MStringU8Chars(path.get());
}

bool CMakeDirectory(const std::string &path) {
    MStringRef dirPath = m_auto_release MStringCreateU8(path.c_str());
    return MMakeDirectory(dirPath.get());
}

std::vector<std::string> CDirectorySubItems(const std::string &path) {
    MStringRef dirPath  = m_auto_release MStringCreateU8(path.c_str());
    MArrayRef  subItems = m_auto_release MCopyPathSubItems(dirPath.get());

    std::vector<std::string> array;

    int count = MArrayLength(subItems.get());
    for (int index = 0; index < count; ++index) {
        auto sub = (MString *)MArrayItem(subItems.get(), index);
        array.push_back(std::string(MStringU8Chars(sub)));
    }
    
    return array;
}

void CRemovePath(const std::string &path) {
    MStringRef filePath = m_auto_release MStringCreateU8(path.c_str());
    MRemovePath(filePath.get());
}

bool CPathExists(const std::string &path, bool *isDirectory) {
    MStringRef filePath = m_auto_release MStringCreateU8(path.c_str());

    bool existing = MPathExists(filePath.get());
    if (existing) {
        if (isDirectory != nullptr) {
            *isDirectory = MDirectoryExists(filePath.get());
        }
        return true;

    } else {
        if (isDirectory != nullptr) {
            *isDirectory = false;
        }
        return false;
    }
}
