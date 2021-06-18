#include "mtypes.h"

//MArray:

MArray *MArray::create() {
    return new MArray();
}

MArrayRef MArray::make() {
    return MArrayRef(create(), MRelease);
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
    return NULL;
}

MArray::~MArray() {
    for (MObject *item : mItems) {
        MRelease(item);
    }
}

//MObject* to MObjectRef:

template<typename T> std::shared_ptr<T> MakeShared(T *value) {
    MRetain(value);
    return std::shared_ptr<T>(value, MRelease);
}

MObjectRef MMakeShared(MObject *value) {return MakeShared(value);}
MBoolRef   MMakeShared(MBool   *value) {return MakeShared(value);}
MIntRef    MMakeShared(MInt    *value) {return MakeShared(value);}
MFloatRef  MMakeShared(MFloat  *value) {return MakeShared(value);}
MStringRef MMakeShared(MString *value) {return MakeShared(value);}
MLambdaRef MMakeShared(MLambda *value) {return MakeShared(value);}
MDataRef   MMakeShared(MData   *value) {return MakeShared(value);}
MArrayRef  MMakeShared(MArray  *value) {return MakeShared(value);}
