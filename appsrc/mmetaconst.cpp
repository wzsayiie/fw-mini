#include "mmetaconst.h"
#include <cstdlib>

m_static_object(sMetaMap (), std::map<std::string, _MConstMeta>)
m_static_object(sIterator(), std::map<std::string, _MConstMeta>::iterator)

void _MConstSetU8Chars(const char *name, const char *value) {
    //intentional memory leak.
    MString *string = MStringCreateU8(value);

    _MConstMeta meta; {
        meta.benchmarkId = MTypeIdOf<MString *>::Value;
        meta.stringValue = string;
        meta.intValue    = atoi(value);
        meta.floatValue  = (float)atof(value);
    }
    sMetaMap()[name] = meta;
}

void _MConstSetU16Chars(const char *name, const char16_t *value) {
    //intentional memory leak.
    MString *string = MStringCreateU16(value);

    const char *chars = MStringU8Chars(string);

    _MConstMeta meta; {
        meta.benchmarkId = MTypeIdOf<MString *>::Value;
        meta.stringValue = string;
        meta.intValue    = atoi(chars);
        meta.floatValue  = (float)atof(chars);
    }
    sMetaMap()[name] = meta;
}

void _MConstSetInt(const char *name, int value) {
    //intentional memory leak.
    MString *string = MStringCreateU8(MFormat("%d", value));

    _MConstMeta meta; {
        meta.benchmarkId = MTypeIdOf<int>::Value;
        meta.stringValue = string;
        meta.intValue    = value;
        meta.floatValue  = (float)value;
    }
    sMetaMap()[name] = meta;
}

void _MConstSetFloat(const char *name, float value) {
    //intentional memory leak.
    MString *string = MStringCreateU8(MFormat("%f", value));

    _MConstMeta meta; {
        meta.benchmarkId = MTypeIdOf<float>::Value;
        meta.stringValue = string;
        meta.intValue    = (int)value;
        meta.floatValue  = value;
    }
    sMetaMap()[name] = meta;
}

void MConstSelectFirst() {
    sIterator() = sMetaMap().begin();
}

bool MConstSelectedValid() {
    return sIterator() != sMetaMap().end();
}

void MConstSelectNext() {
    ++sIterator();
}

bool MConstSelect(const char *name) {
    if (name) {
        sIterator() = sMetaMap().find(name);
    } else {
        sIterator() = sMetaMap().end();
    }

    return sIterator() != sMetaMap().end();
}

const char *MConstSelectedName  () { return sIterator()->first.c_str()     ; }
MTypeId     MConstSelectedTypeId() { return sIterator()->second.benchmarkId; }
MString    *MConstSelectedString() { return sIterator()->second.stringValue; }
int         MConstSelectedInt   () { return sIterator()->second.intValue   ; }
float       MConstSelectedFloat () { return sIterator()->second.floatValue ; }
