#pragma once

#include "jnihelper.h"
#include "mhostapi.h"

class MAndroidImage : public MImage
{
public:
    MAndroidImage(jobject nativeImage);
    ~MAndroidImage();

    jobject nativeImage();

private:
    jobject mNativeImage;
};
