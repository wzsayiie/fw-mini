﻿#include <cstdio>
#include <windowsx.h>
#include "mapp.h"
#include "mhostui.h"
#include "mpaint.h"
#include "mwinapi.h"

static void OpenConsole(void)
{
    AllocConsole();

    FILE* newStdin  = nullptr;
    FILE* newStdout = nullptr;
    FILE* newStderr = nullptr;
    _wfreopen_s(&newStdin , L"conin$" , L"r", stdin );
    _wfreopen_s(&newStdout, L"conout$", L"w", stdout);
    _wfreopen_s(&newStderr, L"conout$", L"w", stderr);

    WCHAR title[MAX_PATH];
    GetConsoleTitleW(title, MAX_PATH);

    HWND hWnd = FindWindowW(nullptr, title);
    SetWindowPos(hWnd, HWND_TOP, 0, 100, 0, 0, SWP_NOSIZE);
}

const UINT_PTR UpdateTimerID = 1;
const UINT_PTR DrawTimerID   = 2;

static void GetClientSize(HWND wnd, LONG *width, LONG *height)
{
    RECT rect = {0};
    GetClientRect(wnd, &rect);

    *width  = rect.right  - rect.left;
    *height = rect.bottom - rect.top ;
}

static LRESULT OnCreate(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    OpenConsole();

    MRegisterApis();
    MPaintStart();

    //application events.
    _MAppLaunch();
    SetTimer(wnd, UpdateTimerID, (UINT)(1000 * _MAppUpdateInterval), nullptr);

    //window events:
    LONG width  = 0;
    LONG height = 0;
    GetClientSize(wnd, &width, &height);
    _MWindowOnResize((_WPIXEL)width, (_WPIXEL)height);

    _MWindowOnLoad();

    SetTimer(wnd, DrawTimerID, (UINT)(1000 * _MWindowDrawInterval), nullptr);

    return 0;
}

static LRESULT OnShowWindow(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    auto shown = (BOOL)wParam;
    if (shown)
    {
        _MWindowOnShow();
    }
    else
    {
        _MWindowOnHide();
    }

    return 0;
}

static LRESULT OnDestroy(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MPaintStop();

    KillTimer(wnd, UpdateTimerID);
    KillTimer(wnd, DrawTimerID);

    PostQuitMessage(0);
    return 0;
}

static LRESULT OnTimer(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    UINT_PTR timerID = wParam;
    if (timerID == UpdateTimerID)
    {
        _MAppUpdate();
    }
    else if (timerID == DrawTimerID)
    {
        InvalidateRect(wnd, nullptr, TRUE);
    }

    return 0;
}

static LRESULT OnSize(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    LPARAM clientSize = lParam;
    float width  = LOWORD(clientSize);
    float height = HIWORD(clientSize);
    _MWindowOnResize(width, height);
    return 0;
}

static LRESULT OnPaint(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    LONG width  = 0;
    LONG height = 0;
    GetClientSize(wnd, &width, &height);

    PAINTSTRUCT paint = {0};
    BeginPaint(wnd, &paint);
    {
        HDC dc = CreateCompatibleDC(paint.hdc);
        HBITMAP bmp = CreateCompatibleBitmap(paint.hdc, width, height);
        SelectBitmap(dc, bmp);

        MPaint(dc);

        BitBlt(paint.hdc, 0, 0, width, height, dc, 0, 0, SRCCOPY);
        DeleteDC(dc);
        DeleteBitmap(bmp);
    }
    EndPaint(wnd, &paint);
    return 0;
}

static bool sLButtonDowned = false;

static LRESULT OnLButtonDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    sLButtonDowned = true;
    
    LPARAM clientPoint = lParam;
    auto x = (float)GET_X_LPARAM(clientPoint);
    auto y = (float)GET_Y_LPARAM(clientPoint);
    _MWindowOnTouchBegin(x, y);

    return 0;
}

static LRESULT OnMouseMove(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    if (sLButtonDowned)
    {
        LPARAM clientPoint = lParam;
        auto x = (float)GET_X_LPARAM(clientPoint);
        auto y = (float)GET_Y_LPARAM(clientPoint);
        _MWindowOnTouchMove(x, y);
    }
    return 0;
}

static LRESULT OnLButtonUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    LPARAM clientPoint = lParam;
    auto x = (float)GET_X_LPARAM(clientPoint);
    auto y = (float)GET_Y_LPARAM(clientPoint);
    _MWindowOnTouchEnd(x, y);

    sLButtonDowned = false;

    return 0;
}

static LRESULT OnKeyDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    WPARAM virutalKey = wParam;
    switch (virutalKey)
    {
        case VK_BACK  : _MWindowOnKeyDown(MKey_Back ); break;
        case VK_RETURN: _MWindowOnKeyDown(MKey_Enter); break;
        case VK_SPACE : _MWindowOnKeyDown(MKey_Space); break;
        case VK_LEFT  : _MWindowOnKeyDown(MKey_Left ); break;
        case VK_UP    : _MWindowOnKeyDown(MKey_Up   ); break;
        case VK_RIGHT : _MWindowOnKeyDown(MKey_Right); break;
        case VK_DOWN  : _MWindowOnKeyDown(MKey_Down ); break;
        case 'A'      : _MWindowOnKeyDown(MKey_A    ); break;
        case 'W'      : _MWindowOnKeyDown(MKey_W    ); break;
        case 'D'      : _MWindowOnKeyDown(MKey_D    ); break;
        case 'S'      : _MWindowOnKeyDown(MKey_S    ); break;
        default       : ;
    }
    return 0;
}

static LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE     : return OnCreate     (wnd, wParam, lParam);
        case WM_SHOWWINDOW : return OnShowWindow (wnd, wParam, lParam);
        case WM_DESTROY    : return OnDestroy    (wnd, wParam, lParam);
        case WM_TIMER      : return OnTimer      (wnd, wParam, lParam);
        case WM_SIZE       : return OnSize       (wnd, wParam, lParam);
        case WM_PAINT      : return OnPaint      (wnd, wParam, lParam);
        case WM_LBUTTONDOWN: return OnLButtonDown(wnd, wParam, lParam);
        case WM_MOUSEMOVE  : return OnMouseMove  (wnd, wParam, lParam);
        case WM_LBUTTONUP  : return OnLButtonUp  (wnd, wParam, lParam);
        case WM_KEYDOWN    : return OnKeyDown    (wnd, wParam, lParam);

        default: return DefWindowProcW(wnd, msg, wParam, lParam);
    }
}

int APIENTRY wWinMain(
    _In_     HINSTANCE instance,
    _In_opt_ HINSTANCE prevInst,
    _In_     LPWSTR    cmdLine ,
    _In_     int       showCmd )
{
    //register window class.
    LPCWSTR className = L"MWindow";
    WNDCLASSW wndClass = {0};

    wndClass.style         = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc   = WindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = instance;
    wndClass.hIcon         = nullptr;
    wndClass.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wndClass.lpszMenuName  = nullptr;
    wndClass.lpszClassName = className;

    RegisterClassW(&wndClass);

    //show window.
    HWND wnd = CreateWindowExW(
        /* dwExStyle    */ 0,
        /* lpClassName  */ className,
        /* lpWindowName */ L"Mini",
        /* dwStyle      */ WS_OVERLAPPEDWINDOW,
        /* x,y          */ 1000, 100,
        /* width,height */  376, 679,
        /* hWndParent   */ nullptr,
        /* hMenu        */ nullptr,
        /* hInstance    */ instance,
        /* lpParam      */ nullptr
    );

    ShowWindow(wnd, showCmd);
    UpdateWindow(wnd);

    //message loop.
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
