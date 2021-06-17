#include <cstdio>
#include <windows.h>
#include "mmain.h"

static void OpenConsole(void)
{
    AllocConsole();

    FILE* newStdin  = NULL;
    FILE* newStdout = NULL;
    FILE* newStderr = NULL;
    _wfreopen_s(&newStdin , L"conin$" , L"r", stdin );
    _wfreopen_s(&newStdout, L"conout$", L"w", stdout);
    _wfreopen_s(&newStderr, L"conout$", L"w", stderr);

    WCHAR title[MAX_PATH];
    GetConsoleTitleW(title, MAX_PATH);

    HWND hWnd = FindWindowW(NULL, title);
    SetWindowPos(hWnd, HWND_TOP, 300, 100, 0, 0, SWP_NOSIZE);
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)
    {
        OpenConsole();
        _MMain();
        return 0;
    }
    else if (msg == WM_PAINT)
    {
        PAINTSTRUCT paint;
        BeginPaint(hwnd, &paint);
        EndPaint(hwnd, &paint);
        return 0;
    }
    else if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    else
    {
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

int APIENTRY wWinMain(
    _In_     HINSTANCE instance,
    _In_opt_ HINSTANCE prevInst,
    _In_     LPWSTR    cmdLine ,
    _In_     int       cmdShow )
{
    //register window class.
    LPCWSTR className = L"MWindow";
    WNDCLASSW wndClass = {0};

    wndClass.style         = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc   = WindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = instance;
    wndClass.hIcon         = NULL;
    wndClass.hCursor       = NULL;
    wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = className;

    RegisterClassW(&wndClass);

    //show window.
    HWND hwnd = CreateWindowExW(
        /* dwExStyle    */ 0,
        /* lpClassName  */ className,
        /* lpWindowName */ L"Mini",
        /* dwStyle      */ WS_OVERLAPPEDWINDOW,
        /* x,y          */   0, 100,
        /* width,height */ 300, 300,
        /* hWndParent   */ NULL,
        /* hMenu        */ NULL,
        /* hInstance    */ instance,
        /* lpParam      */ NULL
    );

    ShowWindow(hwnd, cmdShow);
    UpdateWindow(hwnd);

    //message loop.
    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
