#pragma once

#include "mimage.h"
#include "mjnihelper.h"

minikit_class(MAndrdImage, MImage) {
public:
    jobject mBitmap = nullptr;
};
