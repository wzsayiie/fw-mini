#pragma once

#include <windows.h> //"gdiplus.h" need.
#include <gdiplus.h>
#include "mhostapi.h"

//native types:

class MWin32Image : public MImage
{
public:
    MWin32Image(Gdiplus::Image *gdiImage);
    ~MWin32Image();

    Gdiplus::Image *gdiImage();

private:
    Gdiplus::Image *mGdiImage;
};

//host api:

void MRegisterApi();
