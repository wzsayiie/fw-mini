#include "mwin32gdiplus.h"

static ULONG_PTR sGdiplusToken = 0;

void MWin32GdiplusStartup()
{
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&sGdiplusToken, &input, nullptr);
}

void MWin32GdiplusShutdown()
{
    //Gdiplus::GdiplusShutdown(sGdiplusToken);
    //sGdiplusToken = 0;
}
