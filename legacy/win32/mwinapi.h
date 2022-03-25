// #pragma once

// #include <windows.h> //"gdiplus.h" need.
// #include <gdiplus.h>
// #include "mhostapi.h"

// //native types:

// class MWin32Image : public MImage
// {
// public:
//     MWin32Image(Gdiplus::Image *gdiImage);
//     ~MWin32Image();

//     Gdiplus::Image *gdiImage();

// private:
//     Gdiplus::Image *mGdiImage;
// };

// //color pattern:

// union MWin32ColorPattern
// {
//     struct
//     {
//         uint8_t blue ;
//         uint8_t green;
//         uint8_t red  ;
//         uint8_t alpha;
//     };
//     int color;
// };

// //host api:

// void MRegisterApi();
