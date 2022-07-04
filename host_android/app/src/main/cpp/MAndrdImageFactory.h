#pragma once

#include "mimage.h"
#include "mjnihelper.h"

m_class(MAndrdImage, MImage) {
public:
    std::shared_ptr<_jobject> mBitmap;
};
