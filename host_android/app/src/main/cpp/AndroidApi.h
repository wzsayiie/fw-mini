#pragma once

#include <jni.h>
#include "mtypes.h"

class MImageLoad : public MSpecial
{
public:
    MImageLoad(jobject nativeImage);
    ~MImageLoad();

    jobject nativeImage();

private:
    jobject mNativeImage;
};
