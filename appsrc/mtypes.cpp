#include "mtypes.h"

MArray *MArray::create(const std::initializer_list<MObject *> &items) {
    return new MArray(items);
}

MArrayRef MArray::make(const std::initializer_list<MObject *> &items) {
    return MArrayRef(create(items), MRelease);
}

MArray::MArray(const std::initializer_list<MObject *> &items) : MObject(MType_MArray) {
    for (MObject *item : items) {
        MRetain(item);
        mItems.push_back(item);
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

MArray::~MArray() {
    for (MObject *item : mItems) {
        MRelease(item);
    }
}
