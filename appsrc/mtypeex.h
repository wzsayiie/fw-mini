#pragma once

#include <memory>
#include "mtypes.h"

template<typename T> T *_MAsObject(MObject *object, MType type) {
    if (object && MGetType(object) == type) {
        return (T *)object;
    }
    return nullptr;
}

struct _MAsBoolHelper   { MBool   *operator<<(MObject *a) { return _MAsObject<MBool  >(a, MType_MBool  ); } };
struct _MAsIntHelper    { MInt    *operator<<(MObject *a) { return _MAsObject<MInt   >(a, MType_MInt   ); } };
struct _MAsFloatHelper  { MFloat  *operator<<(MObject *a) { return _MAsObject<MFloat >(a, MType_MFloat ); } };
struct _MAsStringHelper { MString *operator<<(MObject *a) { return _MAsObject<MString>(a, MType_MString); } };
struct _MAsLambdaHelper { MLambda *operator<<(MObject *a) { return _MAsObject<MLambda>(a, MType_MLambda); } };
struct _MAsDataHelper   { MData   *operator<<(MObject *a) { return _MAsObject<MData  >(a, MType_MData  ); } };
struct _MAsArrayHelper  { MArray  *operator<<(MObject *a) { return _MAsObject<MArray >(a, MType_MArray ); } };
struct _MAsImageHelper  { MImage  *operator<<(MObject *a) { return _MAsObject<MImage >(a, MType_MImage ); } };

#define m_as_bool   _MAsBoolHelper  ()<<
#define m_as_int    _MAsIntHelper   ()<<
#define m_as_float  _MAsFloatHelper ()<<
#define m_as_string _MAsStringHelper()<<
#define m_as_lambda _MAsLambdaHelper()<<
#define m_as_data   _MAsDataHelper  ()<<
#define m_as_array  _MAsArrayHelper ()<<
#define m_as_image  _MAsImageHelper ()<<

typedef std::shared_ptr<MObject> MObjectRef;
typedef std::shared_ptr<MBool  > MBoolRef  ;
typedef std::shared_ptr<MInt   > MIntRef   ;
typedef std::shared_ptr<MFloat > MFloatRef ;
typedef std::shared_ptr<MString> MStringRef;
typedef std::shared_ptr<MLambda> MLambdaRef;
typedef std::shared_ptr<MData  > MDataRef  ;
typedef std::shared_ptr<MArray > MArrayRef ;
typedef std::shared_ptr<MImage > MImageRef ;

class _MMakeSharedHelper {

public:
    MObjectRef operator<<(MObject *a) { return makeShared(a); }
    MBoolRef   operator<<(MBool   *a) { return makeShared(a); }
    MIntRef    operator<<(MInt    *a) { return makeShared(a); }
    MFloatRef  operator<<(MFloat  *a) { return makeShared(a); }
    MStringRef operator<<(MString *a) { return makeShared(a); }
    MLambdaRef operator<<(MLambda *a) { return makeShared(a); }
    MDataRef   operator<<(MData   *a) { return makeShared(a); }
    MArrayRef  operator<<(MArray  *a) { return makeShared(a); }
    MImageRef  operator<<(MImage  *a) { return makeShared(a); }

private:
    template<typename T> std::shared_ptr<T> makeShared(T *object) {
        MRetain(object);
        return std::shared_ptr<T>(object, MRelease);
    }
};

#define m_make_shared _MMakeSharedHelper()<<

class _MAutoReleaseHelper {

public:
    MObjectRef operator<<(MObject *a) { return autoRelease(a); }
    MBoolRef   operator<<(MBool   *a) { return autoRelease(a); }
    MIntRef    operator<<(MInt    *a) { return autoRelease(a); }
    MFloatRef  operator<<(MFloat  *a) { return autoRelease(a); }
    MStringRef operator<<(MString *a) { return autoRelease(a); }
    MLambdaRef operator<<(MLambda *a) { return autoRelease(a); }
    MDataRef   operator<<(MData   *a) { return autoRelease(a); }
    MArrayRef  operator<<(MArray  *a) { return autoRelease(a); }
    MImageRef  operator<<(MImage  *a) { return autoRelease(a); }

private:
    template<typename T> std::shared_ptr<T> autoRelease(T *object) {
        return std::shared_ptr<T>(object, MRelease);
    }
};

#define m_auto_release _MAutoReleaseHelper()<<
