#pragma once

#include <memory>
#include "mtypes.h"

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
