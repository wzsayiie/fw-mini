#define _CRT_SECURE_NO_WARNINGS
#include "mresource.h"
#include <cstdio>
#include <string>
#include "mencode.h"
#include "mhostapi.h"

const int ReadFileBufferSize = 8 * 1024;

MData *MCopyDataFromFile(MString *path) {
    if (!path) {
        return nullptr;
    }

    FILE *file = fopen(MStringU8Chars(path), "rb");
    if (!file) {
        return nullptr;
    }

    MData *data = MDataCreate(nullptr, 0);

    uint8_t buffer[ReadFileBufferSize];
    while (!feof(file)) {
        int size = (int)fread(buffer, 1, ReadFileBufferSize, file);
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

static void WriteFile(MString *path, const uint8_t *bytes, int size) {
    if (!path) {
        return;
    }

    FILE *file = fopen(MStringU8Chars(path), "wb");
    if (file) {
        fwrite(bytes, 1, size, file);
        fclose(file);
    }
}

void MWriteDataToFile(MString *path, MData *data) {
    if (data) {
        WriteFile(path, MDataBytes(data), MDataSize(data));
    }
}

void MWriteU8StringToFile(MString *path, MString *string) {
    if (!string) {
        return;
    }

    auto bytes = (const uint8_t *)MStringU8Chars(string);
    int size = MStringU8Size(string);
    WriteFile(path, bytes, size);
}

void MWriteU16StringToFile(MString *path, MString *string) {
    if (!string) {
        return;
    }

    auto bytes = (const uint8_t *)MStringU16Chars(string);
    int size = MStringU16Size(string) * 2;
    WriteFile(path, bytes, size);
}
