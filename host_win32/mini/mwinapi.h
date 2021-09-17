#pragma once

#include <windows.h> //"gdiplus.h" need.
#include <gdiplus.h>
#include "mhostapi.h"

//native types:

class MWin32Image : public MImage {

public:
    MWin32Image(Gdiplus::Image *nativeImage);
    ~MWin32Image();

    Gdiplus::Image *nativeImage();

private:
    Gdiplus::Image *mNativeImage;
};

//host api:

void MRegisterApi();
