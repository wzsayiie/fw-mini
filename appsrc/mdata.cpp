#include "mdata.h"

void MData::appendBytes(uint8_t *begin, uint8_t *end) {
    mBytes.insert(mBytes.end(), begin, end);
}

define_reflectable_class_function(MData, appendData, "args:data");
void MData::appendData(const MData::ptr &data) {
    mBytes.insert(mBytes.end(), data->mBytes.begin(), data->mBytes.end());
}

define_reflectable_class_function(MData, resize, "args:length")
void MData::resize(int length) {
    mBytes.resize((size_t)length);
}

define_reflectable_class_function(MData, length, "getter")
int MData::length() {
    return (int)mBytes.size();
}

define_reflectable_class_function(MData, empty, "getter")
bool MData::empty() {
    return mBytes.empty();
}

define_reflectable_class_function(MData, setByteAt, "args:index,item")
void MData::setByteAt(int index, uint8_t item) {
    mBytes[index] = item;
}

define_reflectable_class_function(MData, byteAt, "args:index")
uint8_t MData::byteAt(int index) {
    return mBytes[index];
}

uint8_t *MData::bytes() {
    return mBytes.data();
}

uint8_t *MData::bytesEnd() {
    return mBytes.data() + mBytes.size();
}
