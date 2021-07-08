#define _CRT_SECURE_NO_WARNINGS
#include "mstdapis.h"
#include <cstdio>

const int READ_BUFFER_SIZE = 8 * 1024;

MData *MCopyFileContent(MString *path) {
    if (!path) {
        return nullptr;
    }

    FILE *file = fopen(path->u8Bytes(), "rb");
    if (!file) {
        return nullptr;
    }

    MData *data = MDataCreate(nullptr, 0);
    uint8_t buffer[READ_BUFFER_SIZE];
    while (!feof(file)) {
        auto size = (int)fread(buffer, 1, READ_BUFFER_SIZE, file);
        MDataAppend(data, buffer, size);
    }

    fclose(file);
    return data;
}

void MWriteFile(MString *path, MData *content) {
    if (!path || !content) {
        return;
    }

    FILE *file = fopen(path->u8Bytes(), "wb");
    if (!file) {
        return;
    }

    const uint8_t *bytes = content->bytes();
    auto size = (int)content->size();
    fwrite(bytes, 1, size, file);

    fclose(file);
}
