#include "mmetaconst.h"
#include <cstdlib>

static def_singleton(sMetaMap , std::map<std::string, _MConstMeta>)
static def_singleton(sIterator, std::map<std::string, _MConstMeta>::iterator)

void _MConstSetU8Chars(const char *name, const char *value, const char *note) {
    //intentional memory leak.
    MString *stringName  = MStringCreateU8(name );
    MString *stringNote  = MStringCreateU8(note );
    MString *stringValue = MStringCreateU8(value);

    _MConstMeta meta; {
        meta.constName   = stringName;
        meta.constNote   = stringNote;
        meta.benchmarkId = MTypeIdOf<MString *>::Value;
        meta.stringValue = stringValue;
        meta.intValue    = atoi(value);
        meta.floatValue  = (float)atof(value);
    }
    sMetaMap()[name] = meta;
}

void _MConstSetU16Chars(const char *name, const char16_t *value, const char *note) {
    //intentional memory leak.
    MString *stringName  = MStringCreateU8 (name );
    MString *stringNote  = MStringCreateU8 (note );
    MString *stringValue = MStringCreateU16(value);

    const char *chars = MStringU8Chars(stringValue);

    _MConstMeta meta; {
        meta.constName   = stringName;
        meta.constNote   = stringNote;
        meta.benchmarkId = MTypeIdOf<MString *>::Value;
        meta.stringValue = stringValue;
        meta.intValue    = atoi(chars);
        meta.floatValue  = (float)atof(chars);
    }
    sMetaMap()[name] = meta;
}

void _MConstSetInt(const char *name, int value, const char *note) {
    //intentional memory leak.
    MString *stringName  = MStringCreateU8(name);
    MString *stringNote  = MStringCreateU8(note);
    MString *stringValue = MStringCreateU8(MFormat("%d", value));

    _MConstMeta meta; {
        meta.constName   = stringName;
        meta.constNote   = stringNote;
        meta.benchmarkId = MTypeIdOf<int>::Value;
        meta.stringValue = stringValue;
        meta.intValue    = value;
        meta.floatValue  = (float)value;
    }
    sMetaMap()[name] = meta;
}

void _MConstSetFloat(const char *name, float value, const char *note) {
    //intentional memory leak.
    MString *stringName  = MStringCreateU8(name);
    MString *stringNote  = MStringCreateU8(note);
    MString *stringValue = MStringCreateU8(MFormat("%f", value));

    _MConstMeta meta; {
        meta.constName   = stringName;
        meta.constNote   = stringNote;
        meta.benchmarkId = MTypeIdOf<float>::Value;
        meta.stringValue = stringValue;
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

MString *MConstSelectedName  () { return sIterator()->second.constName  ; }
MString *MConstSelectedNote  () { return sIterator()->second.constNote  ; }
MTypeId  MConstSelectedTypeId() { return sIterator()->second.benchmarkId; }
MString *MConstSelectedString() { return sIterator()->second.stringValue; }
int      MConstSelectedInt   () { return sIterator()->second.intValue   ; }
float    MConstSelectedFloat () { return sIterator()->second.floatValue ; }
