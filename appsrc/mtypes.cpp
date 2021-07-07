#include "mtypes.h"

MArray::MArray() : MObject(MType_MArray) {
}

MArray::~MArray() {
    for (MObject *item : mItems) {
        MRelease(item);
    }
}

void MArray::append(MObject *item) {
    MRetain(item);
    mItems.push_back(item);
}

int MArray::length() {
    return (int)mItems.size();
}

MObject *MArray::item(int index) {
    if (0 <= index && index < length()) {
        return mItems[index];
    }
    return nullptr;
}
