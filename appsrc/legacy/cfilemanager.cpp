#include "cfilemanager.h"

std::vector<uint8_t> CReadDataFromFile(const std::string &path) {
    MStringRef file = m_auto_release MStringCreateU8(path.c_str());
    MDataRef   data = m_auto_release MCopyDataFromFile(file.get());

    return std::vector<uint8_t>(MDataBytes(data.get()), MDataEnd(data.get()));
}

std::string CReadStringFromFile(const std::string &path) {
    MStringRef file = m_auto_release MStringCreateU8(path.c_str());
    MStringRef text = m_auto_release MCopyStringFromFile(file.get());

    if (text) {
        return std::string(MStringU8Chars(text.get()));
    }
    return "";
}

std::vector<uint8_t> CReadDataFromBundle(const std::string &path) {
    MStringRef file = m_auto_release MStringCreateU8(path.c_str());
    MDataRef   data = m_auto_release MCopyBundleAsset(file.get());

    return std::vector<uint8_t>(MDataBytes(data.get()), MDataEnd(data.get()));
}

std::string CReadStringFromBundle(const std::string &path) {
    MStringRef file = m_auto_release MStringCreateU8(path.c_str());
    MStringRef text = m_auto_release MCopyStringFromBundle(file.get());

    if (text) {
        return std::string(MStringU8Chars(text.get()));
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
