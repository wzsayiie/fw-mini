#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "mconfig.h"

//------------------------------------------------------------------------------
//MObject:

typedef std::function<void (class MObject *object, int refCount)> _MRefObserver;

template<> struct MTypeIdOf<class MObject *> {
    static const MTypeId Value =  MEnumId("Obj");
};

typedef std::shared_ptr<class MObject> MObjectRef;

class MObject {

public:
    void _retain ();
    void _release();
    
    virtual MTypeId _typeId() = 0;

    //for debugging.
    void _setRefObserver(_MRefObserver observer);
    int  _refCount();

protected:
    virtual ~MObject() {}

private:
    _MRefObserver mRefObserver;
    int mRefCount = 1;
};

M_FUNC_EXPORT MObject *MRetain   (MObject *object) MFUNC_META(MRetain   );
M_FUNC_EXPORT void     MRelease  (MObject *object) MFUNC_META(MRelease  );
M_FUNC_EXPORT MTypeId  MGetTypeId(MObject *object) MFUNC_META(MGetTypeId);

//------------------------------------------------------------------------------
//MBool & MInt & MFloat & MPointer:

m_class(MBool   , "Bol") {};
m_class(MInt    , "Int") {};
m_class(MFloat  , "Flt") {};
m_class(MPointer, "Ptr") {};

M_FUNC_EXPORT MBool    *MBoolCreate   (bool     value) MFUNC_META(MBoolCreate   );
M_FUNC_EXPORT MInt     *MIntCreate    (int      value) MFUNC_META(MIntCreate    );
M_FUNC_EXPORT MFloat   *MFloatCreate  (float    value) MFUNC_META(MFloatCreate  );
M_FUNC_EXPORT MPointer *MPointerCreate(uint8_t *value) MFUNC_META(MPointerCreate);

M_FUNC_EXPORT bool     MBoolValue   (MBool    *object) MFUNC_META(MBoolValue   );
M_FUNC_EXPORT int      MIntValue    (MInt     *object) MFUNC_META(MIntValue    );
M_FUNC_EXPORT float    MFloatValue  (MFloat   *object) MFUNC_META(MFloatValue  );
M_FUNC_EXPORT uint8_t *MPointerValue(MPointer *object) MFUNC_META(MPointerValue);

//------------------------------------------------------------------------------
//MString:

m_class(MString, "Str") {};

M_FUNC_EXPORT MString *MStringCreateU8 (const char     *chars) MFUNC_META(MStringCreateU8 );
M_FUNC_EXPORT MString *MStringCreateU16(const char16_t *chars) MFUNC_META(MStringCreateU16);

M_FUNC_EXPORT const char     *MStringU8Chars (MString *string) MFUNC_META(MStringU8Chars );
M_FUNC_EXPORT const char16_t *MStringU16Chars(MString *string) MFUNC_META(MStringU16Chars);

M_FUNC_EXPORT int MStringU8Size (MString *string) MFUNC_META(MStringU8Size );
M_FUNC_EXPORT int MStringU16Size(MString *string) MFUNC_META(MStringU16Size);

//------------------------------------------------------------------------------
//MLambda:

typedef void (*MLambdaFunc)(MObject *load);

m_class(MLambda, "Lmd") {};

M_FUNC_EXPORT MLambda *MLambdaCreate(MLambdaFunc func, MObject *load);
M_FUNC_EXPORT void MLambdaCall(MLambda *lambda) MFUNC_META(MLambdaCall);

//------------------------------------------------------------------------------
//MData:

m_class(MData, "Dat") {};

M_FUNC_EXPORT MData *MDataCreate(const uint8_t *bytes, int size)              MFUNC_META(MDataCreate);
M_FUNC_EXPORT void   MDataAppend(MData *data, const uint8_t *bytes, int size) MFUNC_META(MDataAppend);
M_FUNC_EXPORT int    MDataSize  (MData *data)                                 MFUNC_META(MDataSize  );

M_FUNC_EXPORT const uint8_t *MDataBytes(MData *data) MFUNC_META(MDataBytes);

//------------------------------------------------------------------------------
//MArray:

m_class(MArray, "Arr") {};

M_FUNC_EXPORT MArray  *MArrayCreate()                             MFUNC_META(MArrayCreate);
M_FUNC_EXPORT void     MArrayAppend(MArray *array, MObject *item) MFUNC_META(MArrayAppend);
M_FUNC_EXPORT int      MArrayLength(MArray *array)                MFUNC_META(MArrayLength);
M_FUNC_EXPORT MObject *MArrayItem  (MArray *array, int index)     MFUNC_META(MArrayItem  );

//------------------------------------------------------------------------------
//MUnknown:

m_class(MUnknown, "Uno") {};

//------------------------------------------------------------------------------
//smart pointer:

struct _MMakeSharedHelper {
    template<typename T> std::shared_ptr<T> operator<<(T *object) {
        MRetain(object);
        return std::shared_ptr<T>(object, MRelease);
    }
};
#define m_make_shared _MMakeSharedHelper()<<

struct _MAutoReleaseHelper {
    template<typename T> std::shared_ptr<T> operator<<(T *object) {
        return std::shared_ptr<T>(object, MRelease);
    }
};
#define m_auto_release _MAutoReleaseHelper()<<

//------------------------------------------------------------------------------
//lambda cast:

struct _MLambdaCastHelper {
    MLambdaRef operator<<(std::function<void ()> func);
};
#define m_cast_lambda _MLambdaCastHelper()<<
