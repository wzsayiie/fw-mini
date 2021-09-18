#include "mmetaconst.h"
#include <cstdlib>
#include "mencode.h"
#include "mformat.h"

template<typename T> const T *csdup(const T *src) {
    size_t size = 0;
    while (src[size]) {
        size += 1;
    }
    size += 1; //'\0' at the end.

    auto copy = new T[size];
    if (!copy) {
        return nullptr;
    }
    for (size_t it = 0; it < size; ++it) {
        copy[it] = src[it];
    }
    return copy;
}

m_static_object(sMetaMap (), std::map<std::string, _MConstMeta>)
m_static_object(sIterator(), std::map<std::string, _MConstMeta>::iterator)

void _MConstSetU8Chars(const char *name, const char *value) {

    std::u16string  u16string    = MU16StringFromU8(value);
    const char     *u8charsCopy  = csdup(value);
    const char16_t *u16charsCopy = csdup(u16string.c_str());

    _MConstMeta meta;
    meta.benchmarkId = MTypeIdOf<char *>::Value;
    meta.u8chars     = u8charsCopy;
    meta.u16chars    = u16charsCopy;
    meta.intValue    = atoi(value);
    meta.floatValue  = (float)atof(value);

    sMetaMap()[name] = meta;
}

void _MConstSetU16Chars(const char *name, const char16_t *value) {

    std::string     u8string     = MU8StringFromU16(value);
    const char     *u8charsCopy  = csdup(u8string.c_str());
    const char16_t *u16charsCopy = csdup(value);

    _MConstMeta meta;
    meta.benchmarkId = MTypeIdOf<char16_t *>::Value;
    meta.u8chars     = u8charsCopy;
    meta.u16chars    = u16charsCopy;
    meta.intValue    = atoi(u8charsCopy);
    meta.floatValue  = (float)atof(u8charsCopy);

    sMetaMap()[name] = meta;
}

void _MConstSetInt(const char *name, int value) {

    const char     *u16chars     = MFormat("%d", value);
    std::u16string  u16string    = MU16StringFromU8(u16chars);
    const char     *u8charsCopy  = csdup(u16chars);
    const char16_t *u16charsCopy = csdup(u16string.c_str());

    _MConstMeta meta;
    meta.benchmarkId = MTypeIdOf<int>::Value;
    meta.u8chars     = u8charsCopy;
    meta.u16chars    = u16charsCopy;
    meta.intValue    = value;
    meta.floatValue  = (float)value;

    sMetaMap()[name] = meta;
}

void _MConstSetFloat(const char *name, float value) {

    const char     *u8chars      = MFormat("%f", value);
    std::u16string  u16string    = MU16StringFromU8(u8chars);
    const char     *u8charsCopy  = csdup(u8chars);
    const char16_t *u16charsCopy = csdup(u16string.c_str());

    _MConstMeta meta;
    meta.benchmarkId = MTypeIdOf<float>::Value;
    meta.u8chars     = u8charsCopy;
    meta.u16chars    = u16charsCopy;
    meta.intValue    = (int)value;
    meta.floatValue  = value;

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

const char     *MConstSelectedName    () { return sIterator()->first.c_str()     ; }
MTypeId         MConstSelectedTypeId  () { return sIterator()->second.benchmarkId; }
const char     *MConstSelectedU8Chars () { return sIterator()->second.u8chars    ; }
const char16_t *MConstSelectedU16Chars() { return sIterator()->second.u16chars   ; }
int             MConstSelectedInt     () { return sIterator()->second.intValue   ; }
float           MConstSelectedFloat   () { return sIterator()->second.floatValue ; }
