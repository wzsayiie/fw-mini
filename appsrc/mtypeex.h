#pragma once

#include <memory>
#include "mtypes.h"

template<typename T> T *_MAsObject(MObject *object, MType type) {
    if (object && MGetType(object) == type) {
        return (T *)object;
    }
    return nullptr;
}

inline MBool   *MAsBool  (MObject *a) { return _MAsObject<MBool  >(a, MType_MBool  ); };
inline MInt    *MAsInt   (MObject *a) { return _MAsObject<MInt   >(a, MType_MInt   ); };
inline MFloat  *MAsFloat (MObject *a) { return _MAsObject<MFloat >(a, MType_MFloat ); };
inline MString *MAsString(MObject *a) { return _MAsObject<MString>(a, MType_MString); };
inline MLambda *MAsLambda(MObject *a) { return _MAsObject<MLambda>(a, MType_MLambda); };
inline MData   *MAsData  (MObject *a) { return _MAsObject<MData  >(a, MType_MData  ); };
inline MArray  *MAsArray (MObject *a) { return _MAsObject<MArray >(a, MType_MArray ); };
inline MImage  *MAsImage (MObject *a) { return _MAsObject<MImage >(a, MType_MImage ); };

typedef std::shared_ptr<MObject> MObjectRef;
typedef std::shared_ptr<MBool  > MBoolRef  ;
typedef std::shared_ptr<MInt   > MIntRef   ;
typedef std::shared_ptr<MFloat > MFloatRef ;
typedef std::shared_ptr<MString> MStringRef;
typedef std::shared_ptr<MLambda> MLambdaRef;
typedef std::shared_ptr<MData  > MDataRef  ;
typedef std::shared_ptr<MArray > MArrayRef ;
typedef std::shared_ptr<MImage > MImageRef ;

template<typename T> std::shared_ptr<T> _MMakeShared(T *value) {
    MRetain(value);
    return std::shared_ptr<T>(value, MRelease);
}

inline MObjectRef MMakeShared(MObject *v) { return _MMakeShared(v); }
inline MBoolRef   MMakeShared(MBool   *v) { return _MMakeShared(v); }
inline MIntRef    MMakeShared(MInt    *v) { return _MMakeShared(v); }
inline MFloatRef  MMakeShared(MFloat  *v) { return _MMakeShared(v); }
inline MStringRef MMakeShared(MString *v) { return _MMakeShared(v); }
inline MLambdaRef MMakeShared(MLambda *v) { return _MMakeShared(v); }
inline MDataRef   MMakeShared(MData   *v) { return _MMakeShared(v); }
inline MArrayRef  MMakeShared(MArray  *v) { return _MMakeShared(v); }
inline MImageRef  MMakeShared(MImage  *v) { return _MMakeShared(v); }

template<typename T> std::shared_ptr<T> _MAutorelease(T *value) {
    return std::shared_ptr<T>(value, MRelease);
}

inline MObjectRef MAutorelease(MObject *v) { return _MAutorelease(v); }
inline MBoolRef   MAutorelease(MBool   *v) { return _MAutorelease(v); }
inline MIntRef    MAutorelease(MInt    *v) { return _MAutorelease(v); }
inline MFloatRef  MAutorelease(MFloat  *v) { return _MAutorelease(v); }
inline MStringRef MAutorelease(MString *v) { return _MAutorelease(v); }
inline MLambdaRef MAutorelease(MLambda *v) { return _MAutorelease(v); }
inline MDataRef   MAutorelease(MData   *v) { return _MAutorelease(v); }
inline MArrayRef  MAutorelease(MArray  *v) { return _MAutorelease(v); }
inline MImageRef  MAutorelease(MImage  *v) { return _MAutorelease(v); }
