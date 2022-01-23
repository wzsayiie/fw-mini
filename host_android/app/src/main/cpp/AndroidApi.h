#pragma once

#include "jnihelper.h"
#include "mhostapi.h"

class MAndroidImage : public MImage
{
public:
    MAndroidImage(jobject javaImage);
    ~MAndroidImage();

    jobject javaImage();

private:
    jobject mJavaImage;
};

union MAndroidColorPattern
{
    struct
    {
        uint8_t blue ;
        uint8_t green;
        uint8_t red  ;
        uint8_t alpha;
    };
    int color = 0;
};
