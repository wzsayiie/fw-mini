#pragma once

#include "mobject.h"

m_class(MData, MObject) {
public:
    MData();
    MData(uint8_t *begin, uint8_t *end);

public:
    void appendBytes(uint8_t *begin, uint8_t *end);
    void appendData (const MData::ptr &data);

    void resize(int length);
    int  length();
    bool empty ();

    void setByteAt(int index, uint8_t item);
    uint8_t byteAt(int index);

    uint8_t *bytes   ();
    uint8_t *bytesEnd();

private:
    std::vector<uint8_t> mBytes;
};
