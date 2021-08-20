#pragma once

#include <windows.h> //"gdiplus.h" need.
#include <gdiplus.h>
#include "mtypes.h"

//native types:

class MImageLoad : public MSpecial {

public:
    MImageLoad(Gdiplus::Image *nativeImage);
    ~MImageLoad();

    Gdiplus::Image *nativeImage();

private:
    Gdiplus::Image *mNativeImage;
};

//host api:

void MRegisterApi();
