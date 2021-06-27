#include <cstdio>
#include <windows.h>
#include <windowsx.h>
#include "mapp.h"
#include "mhostui.h"

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

const UINT_PTR TimerID = 0;

static LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    OpenConsole();

    //application events.
    _MAppLaunch();

    //window events.
    RECT rect = {0};
    GetClientRect(hwnd, &rect);
    auto clientW = (float)(rect.right - rect.left);
    auto clientH = (float)(rect.bottom - rect.top);
    _MWindowOnResize(clientW, clientH);
    _MWindowOnLoad();

    SetTimer(hwnd, TimerID, (UINT)(1000 * _MAppUpdateInterval), NULL);

    return 0;
}

static LRESULT OnShowWindow(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    BOOL shown = (BOOL)wParam;
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

static LRESULT OnDestroy(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    KillTimer(hwnd, TimerID);
    PostQuitMessage(0);
    return 0;
}

static LRESULT OnTimer(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    _MAppUpdate();
    return 0;
}

static LRESULT OnSize(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    float clientW = LOWORD(lParam);
    float clientH = HIWORD(lParam);
    _MWindowOnResize(clientW, clientH);
    return 0;
}

static LRESULT OnPaint(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    _MWindowOnDraw();

    PAINTSTRUCT paint;
    BeginPaint(hwnd, &paint);
    EndPaint(hwnd, &paint);
    return 0;
}

static bool sLButtonDowned = false;

static LRESULT OnLButtonDown(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    sLButtonDowned = true;
    
    auto clientX = (float)GET_X_LPARAM(lParam);
    auto clientY = (float)GET_Y_LPARAM(lParam);
    _MWindowOnTouchBegin(clientX, clientY);

    return 0;
}

static LRESULT OnMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if (sLButtonDowned)
    {
        auto clientX = (float)GET_X_LPARAM(lParam);
        auto clientY = (float)GET_Y_LPARAM(lParam);
        _MWindowOnTouchMove(clientX, clientY);
    }
    return 0;
}

static LRESULT OnLButtonUp(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    auto clientX = (float)GET_X_LPARAM(lParam);
    auto clientY = (float)GET_Y_LPARAM(lParam);
    _MWindowOnTouchEnd(clientX, clientY);

    sLButtonDowned = false;

    return 0;
}

static LRESULT OnKeyDown(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
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

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE     : return OnCreate     (hwnd, wParam, lParam);
        case WM_SHOWWINDOW : return OnShowWindow (hwnd, wParam, lParam);
        case WM_DESTROY    : return OnDestroy    (hwnd, wParam, lParam);
        case WM_TIMER      : return OnTimer      (hwnd, wParam, lParam);
        case WM_SIZE       : return OnSize       (hwnd, wParam, lParam);
        case WM_PAINT      : return OnPaint      (hwnd, wParam, lParam);
        case WM_LBUTTONDOWN: return OnLButtonDown(hwnd, wParam, lParam);
        case WM_MOUSEMOVE  : return OnMouseMove  (hwnd, wParam, lParam);
        case WM_LBUTTONUP  : return OnLButtonUp  (hwnd, wParam, lParam);
        case WM_KEYDOWN    : return OnKeyDown    (hwnd, wParam, lParam);

        default: return DefWindowProcW(hwnd, msg, wParam, lParam);
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
