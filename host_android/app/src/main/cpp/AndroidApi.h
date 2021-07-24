#pragma once

#include <jni.h>
#include "mtypes.h"

class MImageLoad : public _MNative {

public:
    MImageLoad(jobject nativeImage);
    ~MImageLoad();

    jobject nativeImage();

private:
    jobject mNativeImage;
};
