#include "mtypes.h"
#include "mencode.h"

//MObject:

MType MGetType(MObject *object) {
    if (object) {
        return object->Type;
    }
    return 0;
}

void MRetain(MObject *object) {
    if (object) {
        object->RefCount += 1;
    }
}

void MRelease(MObject *object) {
    if (!object) {
        return;
    }

    if (--(object->RefCount) == 0) {
        delete object;
    }
}

//MBool:

MBool *MBoolCreate(bool value) {
    MBool *object = new MBool;
    object->Value = value;
    return object;
}

int MBoolValue(MBool *object) {
    if (object) {
        return object->Value;
    }
    return false;
}

//MInt:

MInt *MIntCreate(int value) {
    MInt *object = new MInt;
    object->Value = value;
    return object;
}

int MIntValue(MInt *object) {
    if (object) {
        return object->Value;
    }
    return 0;
}

//MFloat:

MFloat *MFloatCreate(float value) {
    MFloat *object = new MFloat;
    object->Value = value;
    return object;
}

float MFloatValue(MFloat *object) {
    if (object) {
        return object->Value;
    }
    return 0;
}

//MString:

MString *MStringCreateU16s(const char16_t *chars) {
    auto string = new MString;
    if (chars) {
        string->U8string  = MU8stringFromU16s(chars);
        string->U16string = chars;
    }
    return string;
}

MString *MStringCreateU8s(const char *chars) {
    auto string = new MString;
    if (chars) {
        string->U8string  = chars;
        string->U16string = MU16stringFromU8s(chars);
    }
    return string;
}

const char16_t *MStringU16Bytes(MString *string) {
    if (string) {
        return string->U16string.c_str();
    }
    return 0;
}

const char *MStringU8Bytes(MString *string) {
    if (string) {
        return string->U8string.c_str();
    }
    return 0;
}

int MStringU16Size(MString *string) {
    if (string) {
        return (int)string->U16string.size();
    }
    return 0;
}

int MStringU8Size(MString *string) {
    if (string) {
        return (int)string->U8string.size();
    }
    return 0;
}

//MArray:

MArray *MArrayCreate() {
    return new MArray;
}

void MArrayAppend(MArray *array, MObject *item) {
    if (array) {
        MRetain(item);
        array->Items.push_back(item);
    }
}

int MArrayLength(MArray *array) {
    if (array) {
        return (int)array->Items.size();
    }
    return 0;
}

MObject *MArrayItem(MArray *array, int index) {
    if (!array) {
        return NULL;
    }

    auto &items = array->Items;
    if (0 <= index && index < items.size()) {
        return items[index];
    } else {
        return NULL;
    }
}

MArray::~MArray() {
    for (MObject *item : Items) {
        MRelease(item);
    }
}

//MLambda:

MLambda *MLambdaCreate(MLambdaFunc func, MObject *load) {
    if (!func) {
        return NULL;
    }

    MRetain(load);

    auto lambda = new MLambda;
    lambda->Func = func;
    lambda->Load = load;
    return lambda;
}

void MLambdaCall(MLambda *lambda, MObject *param) {
    if (lambda) {
        lambda->Func(lambda->Load, param);
    }
}

MLambda::~MLambda() {
    MRelease(Load);
}

//MData:

MData *MDataCreate(uint8_t *bytes, int size) {
    if (!bytes || size <= 0) {
        return NULL;
    }

    auto data = new MData;
    data->Bytes.insert(data->Bytes.end(), bytes, bytes + size);
    return data;
}

const uint8_t *MDataBytes(MData *data) {
    if (data) {
        return &data->Bytes[0];
    }
    return NULL;
}

int MDataSize(MData *data) {
    if (data) {
        return (int)data->Bytes.size();
    }
    return 0;
}
