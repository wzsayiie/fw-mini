#define _CRT_SECURE_NO_WARNINGS
#include "mclib.h"
#include <cstdio>

const int ReadFileBufferSize = 8 * 1024;

MData *MCopyFileContent(MString *path) {
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
        auto size = (int)fread(buffer, 1, ReadFileBufferSize, file);
        MDataAppend(data, buffer, size);
    }

    fclose(file);
    return data;
}

void MWriteFile(MString *path, MData *content) {
    if (!path || !content) {
        return;
    }

    FILE *file = fopen(MStringU8Chars(path), "wb");
    if (!file) {
        return;
    }

    const uint8_t *bytes = MDataBytes(content);
    int size = MDataSize(content);
    fwrite(bytes, 1, size, file);

    fclose(file);
}
