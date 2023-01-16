#include "mwin32gdiplus.h"
#include <Windows.h> //"gdiplus.h" need.
#include <gdiplus.h>

static ULONG_PTR sGdiplusToken = 0;

void MWin32GdiplusStartup()
{
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&sGdiplusToken, &input, nullptr);
}

void MWin32GdiplusShutdown()
{
    //NOTE: do not shotdown gdi+ manually.
    //some objects may hold gdi+ objects, until destruct at the end of program.
    //
    //Gdiplus::GdiplusShutdown(sGdiplusToken);
    //sGdiplusToken = 0;
}
