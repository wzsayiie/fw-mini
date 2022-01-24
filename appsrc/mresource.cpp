#define _CRT_SECURE_NO_WARNINGS

#include "mresource.h"
#include <filesystem>
#include "mencode.h"

MData *MCopyDataFromFile(MString *path) {
    if (!path) {
        return nullptr;
    }

    FILE *file = fopen(MStringU8Chars(path), "rb");
    if (!file) {
        return nullptr;
    }

    MData *data = MDataCreate(0);

    uint8_t buffer[1024 * 8];
    while (!feof(file)) {
        int size = (int)fread(buffer, 1, sizeof(buffer), file);
        MDataAppend(data, buffer, size);
    }
    fclose(file);

    return data;
}

static MString *CopyString(MData *data) {
    const uint8_t *bytes = MDataBytes(data);
    int size = MDataSize(data);
    if (!bytes || size == 0) {
        return nullptr;
    }

    //assuming this is a utf-8 string
    std::string u8string;

    auto u8begin = (const char *)bytes;
    auto u8end   = (const char *)bytes + size;

    while (u8begin < u8end) {
        char32_t chr = 0;
        int size = MReadU8(u8begin, u8end, &chr);

        if (size > 0) {
            u8string.append(u8begin, u8begin + size);
            u8begin += size;
        } else {
            break;
        }
    }

    if (u8begin == u8end) {
        return MStringCreateU8(u8string.c_str());
    }

    //assuming this is a utf-16 string
    std::u16string u16string;

    auto u16begin = (const char16_t *)bytes;
    auto u16end   = (const char16_t *)bytes + size / 2;

    while (u16begin < u16end) {
        char32_t chr = 0;
        int size = MReadU16(u16begin, u16end, &chr);

        if (size > 0) {
            u16string.append(u16begin, u16begin + size);
            u16begin += size;
        } else {
            break;
        }
    }

    if (u16begin == u16end) {
        return MStringCreateU16(u16string.c_str());
    }

    //return the longer one.
    if (u8string.size() >= u16string.size()) {
        return MStringCreateU8(u8string.c_str());
    } else {
        return MStringCreateU16(u16string.c_str());
    }
}

MString *MCopyStringFromFile(MString *path) {
    MDataRef data = m_auto_release MCopyDataFromFile(path);
    return CopyString(data.get());
}

MString *MCopyStringFromBundle(MString *path) {
    MDataRef data = m_auto_release MCopyBundleAsset(path);
    return CopyString(data.get());
}

MImage *MCreateImageFromFile(MString *path) {
    MDataRef data = m_auto_release MCopyDataFromFile(path);
    return MCreateImage(data.get());
}

MImage *MCreateImageFromBundle(MString *path) {
    MDataRef data = m_auto_release MCopyBundleAsset(path);
    return MCreateImage(data.get());
}

static bool WriteFile(MString *path, const uint8_t *bytes, int size) {
    if (!path) {
        return false;
    }

    FILE *file = fopen(MStringU8Chars(path), "wb");
    if (!file) {
        return false;
    }

    fwrite(bytes, 1, size, file);
    fclose(file);
    return true;
}

bool MWriteDataToFile(MString *path, MData *data) {
    if (data) {
        return WriteFile(path, MDataBytes(data), MDataSize(data));
    } else {
        return false;
    }
}

bool MWriteU8StringToFile(MString *path, MString *string) {
    if (!string) {
        return false;
    }

    auto bytes = (const uint8_t *)MStringU8Chars(string);
    int  size  = MStringU8Size(string);

    return WriteFile(path, bytes, size);
}

bool MWriteU16StringToFile(MString *path, MString *string) {
    if (!string) {
        return false;
    }

    auto bytes = (const uint8_t *)MStringU16Chars(string);
    int  size  = MStringU16Size(string) * 2;
    
    return WriteFile(path, bytes, size);
}

void MMakeDirectores(MString *path) {
    if (path) {
        const char *file = MStringU8Chars(path);
        std::filesystem::create_directories(file);
    }
}

MArray *MCopyDirSubitems(MString *path) {
    if (!MDirectoryExists(path)) {
        return nullptr;
    }

    const char *dir = MStringU8Chars(path);
    std::filesystem::directory_iterator iterator(dir);

    MArray *items = MArrayCreate();
    for (auto &entry : iterator) {
        std::string name = entry.path().filename().string();
        MStringRef item = m_auto_release MStringCreateU8(name.c_str());
        MArrayAppend(items, item.get());
    }
    return items;
}

void MRemovePath(MString *path) {
    if (path) {
        const char *file = MStringU8Chars(path);
        std::filesystem::remove_all(file);
    }
}

static bool PathExists(MString *path, bool *isDirectory) {
    if (!path) {
        return false;
    }

    const char *file = MStringU8Chars(path);
    if (!std::filesystem::exists(file)) {
        return false;
    }

    if (isDirectory != nullptr) {
        *isDirectory = std::filesystem::is_directory(file);
    }
    return true;
}

bool MPathExists(MString *path) {
    return PathExists(path, nullptr);
}

bool MDirectoryExists(MString *path) {
    bool isDirectory = false;
    bool isExisting  = PathExists(path, &isDirectory);
    return isExisting && isDirectory;
}

bool MFileExists(MString *path) {
    bool isDirectory = false;
    bool isExisting  = PathExists(path, &isDirectory);
    return isExisting && !isDirectory;
}
