#include "mtypes.h"

MArray::MArray() : MObject(MType_MArray) {
}

MArray::~MArray() {
    for (MObject *item : mItems) {
        MRelease(item);
    }
}

void MArray::_append(MObject *item) {
    MRetain(item);
    mItems.push_back(item);
}

int MArray::_length() {
    return (int)mItems.size();
}

MObject *MArray::_item(int index) {
    if (0 <= index && index < _length()) {
        return mItems[index];
    }
    return nullptr;
}
