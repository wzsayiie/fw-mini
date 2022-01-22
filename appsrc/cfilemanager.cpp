#include "cfilemanager.h"

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
    MDataRef   fileData = m_auto_release MDataCopy(&data[0], (int)data.size());

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
