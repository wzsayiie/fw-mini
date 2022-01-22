﻿#include <clocale>
#include <io.h>
#include <windowsx.h>
#include "mhostloop.h"
#include "mhostui.h"
#include "mjsrt.h"
#include "mpaint.h"
#include "mwinapi.h"

const int ConsoleFrameX     =   20;
const int ConsoleFrameY     =  100;
const int WindowFrameX      = 1000;
const int WindowFrameY      =  100;
const int WindowFrameWidth  =  376;
const int WindowFrameHeight =  679;

static void OpenConsole(void)
{
    //is there a console?
    if (_isatty(_fileno(stdout)))
    {
        return;
    }

    AllocConsole();

    FILE* newStdin  = nullptr;
    FILE* newStdout = nullptr;
    FILE* newStderr = nullptr;
    _wfreopen_s(&newStdin , L"conin$" , L"r", stdin );
    _wfreopen_s(&newStdout, L"conout$", L"w", stdout);
    _wfreopen_s(&newStderr, L"conout$", L"w", stderr);

    //IMPORTANT: use local character set.
    setlocale(LC_CTYPE, "");

    HWND wnd = nullptr;
    {
        WCHAR title[MAX_PATH] = L"\0";
        GetConsoleTitleW(title, MAX_PATH);
        wnd = FindWindowW(nullptr, title);
    }

    //move the console window.
    SetWindowPos(wnd, HWND_TOP, ConsoleFrameX, ConsoleFrameY, 0, 0, SWP_NOSIZE);

    //disable the close button of console window.
    //clicking it will cause the program to exit abnormally.
    HMENU menu = GetSystemMenu(wnd, FALSE);
    EnableMenuItem(menu, SC_CLOSE, MF_DISABLED);
}

static void GetClientSize(HWND wnd, int *width, int *height)
{
    RECT rect = {0};
    GetClientRect(wnd, &rect);

    *width  = rect.right  - rect.left;
    *height = rect.bottom - rect.top ;
}

static MString *CopyEditText(HWND edit)
{
    const int BufferSize = 1024;
    
    WCHAR buffer[BufferSize] = L"";
    GetWindowTextW(edit, buffer, BufferSize);

    return MStringCreateU16((const char16_t *)buffer);
}

const UINT_PTR UpdateTimerID = 1;
const UINT_PTR DrawTimerID   = 2;

static HWND    sEditWnd         = nullptr;
static WNDPROC sEditDefaultProc = nullptr;
static bool    sLButtonDowned   = false;

static LRESULT CALLBACK EditCustomProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KEYDOWN && wParam == VK_RETURN)
    {
        MStringRef text = m_auto_release CopyEditText(wnd);
        _MWindowOnTextBox(text.get(), true);
        return 0;
    }
    else
    {
        return sEditDefaultProc(wnd, msg, wParam, lParam);
    }
}

static void CreateEditWithParent(HWND parent)
{
    sEditWnd = CreateWindowExW(
        /* dwExStyle    */ 0,
        /* lpClassName  */ L"edit",
        /* lpWindowName */ nullptr,
        /* dwStyle      */ WS_CHILD | WS_BORDER | ES_WANTRETURN,
        /* x,y          */ 0, 0,
        /* width,height */ 0, 0,
        /* hWndParent   */ parent,
        /* hMenu        */ nullptr,
        /* hInstance    */ nullptr,
        /* lpParam      */ nullptr
    );

    sEditDefaultProc = (WNDPROC)GetWindowLongPtrW(sEditWnd, GWLP_WNDPROC);
    SetWindowLongPtrW(sEditWnd, GWLP_WNDPROC, (LONG_PTR)EditCustomProc);
}

static void AdjustEditPosition(int parentWidth, int parentHeight)
{
    const int Width  = 200;
    const int Height =  20;

    int x = (parentWidth - Width) / 2;
    int y = parentHeight - Height - 20;
    SetWindowPos(sEditWnd, nullptr, x, y, Width, Height, 0);
}

static void UpdateEditState()
{
    if (!_MWindowTextBoxUpdated())
    {
        return;
    }

    //IMPORTANT: reset the flag.
    MWindowSetTextBoxUpdated(false);

    if (_MWindowTextBoxEnabled())
    {
        MString *textBoxString  = _MWindowTextBoxRawString();
        WPARAM   selectionBegin = 0;
        LPARAM   selectionEnd   = MStringU16Size(textBoxString);

        SetWindowTextW(sEditWnd, (LPWSTR)MStringU16Chars(textBoxString));
        SendMessageW(sEditWnd, EM_SETSEL, selectionBegin, selectionEnd);

        ShowWindow(sEditWnd, SW_SHOW);
        SetFocus(sEditWnd);
    }
    else
    {
        ShowWindow(sEditWnd, SW_HIDE);
    }
}

static LRESULT OnCreate(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    OpenConsole();
    MPaintStart();

    MInstallJSRuntime();
    MRegisterApi();

    int width  = 0;
    int height = 0;
    GetClientSize(wnd, &width, &height);

    //create edit control.
    CreateEditWithParent(wnd);
    AdjustEditPosition(width, height);

    //application events.
    _MAppLaunch();
    SetTimer(wnd, UpdateTimerID, (UINT)(1000 * _MAppUpdateInterval), nullptr);

    //window events:
    _MWindowOnResize((_MPixel)width, (_MPixel)height);
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
        UpdateEditState();
        _MAppUpdate();
    }
    else if (timerID == DrawTimerID)
    {
        InvalidateRect(wnd, nullptr, FALSE);
    }

    return 0;
}

static LRESULT OnSize(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    LPARAM clientSize = lParam;
    int width  = LOWORD(clientSize);
    int height = HIWORD(clientSize);

    AdjustEditPosition(width, height);
    _MWindowOnResize((_MPixel)width, (_MPixel)height);
    
    return 0;
}

static LRESULT OnPaint(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    int width  = 0;
    int height = 0;
    GetClientSize(wnd, &width, &height);

    PAINTSTRUCT paint = {0};
    BeginPaint(wnd, &paint);
    {
        HDC dc = CreateCompatibleDC(paint.hdc);
        HBITMAP bmp = CreateCompatibleBitmap(paint.hdc, width, height);
        SelectBitmap(dc, bmp);

        //set the background gray.
        RECT rect = { 0, 0, width, height };
        HBRUSH backgroundBrush = CreateSolidBrush(0xF0F0F0);
        FillRect(dc, &rect, backgroundBrush);
        DeleteBrush(backgroundBrush);

        MPaint(dc);

        BitBlt(paint.hdc, 0, 0, width, height, dc, 0, 0, SRCCOPY);
        DeleteDC(dc);
        DeleteBitmap(bmp);
    }
    EndPaint(wnd, &paint);
    return 0;
}

static LRESULT OnLButtonDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    LPARAM clientPoint = lParam;
    auto x = (_MPixel)GET_X_LPARAM(clientPoint);
    auto y = (_MPixel)GET_Y_LPARAM(clientPoint);

    //mouse event.
    _MWindowOnMouseMove(x, y);

    //touch event:
    sLButtonDowned = true;
    _MWindowOnTouchBegin(x, y);

    //NOTE: to capture events when the mouse moves outside the window.
    SetCapture(wnd);

    return 0;
}

static LRESULT OnMouseMove(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    LPARAM clientPoint = lParam;
    int x = GET_X_LPARAM(clientPoint);
    int y = GET_Y_LPARAM(clientPoint);

    //mouse event:
    int width  = 0;
    int height = 0;
    GetClientSize(wnd, &width, &height);

    if (0 <= x && x <= width
     && 0 <= y && y <= height)
    {
        _MWindowOnMouseMove((_MPixel)x, (_MPixel)y);
    }

    //touch event.
    if (sLButtonDowned)
    {
        _MWindowOnTouchMove((_MPixel)x, (_MPixel)y);
    }

    return 0;
}

static LRESULT OnLButtonUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    LPARAM clientPoint = lParam;
    int x = GET_X_LPARAM(clientPoint);
    int y = GET_Y_LPARAM(clientPoint);

    //mouse event:
    int width  = 0;
    int height = 0;
    GetClientSize(wnd, &width, &height);

    if (0 <= x && x <= width
     && 0 <= y && y <= height)
    {
        _MWindowOnMouseMove((_MPixel)x, (_MPixel)y);
    }

    //touch event.
    _MWindowOnTouchEnd((_MPixel)x, (_MPixel)y);
    sLButtonDowned = false;
    ReleaseCapture();

    return 0;
}

static LRESULT OnMouseWheel(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    short delta = GET_WHEEL_DELTA_WPARAM(wParam);
    _MWindowOnMouseWheel(delta);

    return 0;
}

static LRESULT OnCommand(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    HWND controlWnd = (HWND)(lParam);
    int  notifyCode = HIWORD(wParam);

    if (controlWnd == sEditWnd && notifyCode == EN_CHANGE)
    {
        MStringRef text = m_auto_release CopyEditText(sEditWnd);
        _MWindowOnTextBox(text.get(), false);
    }

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

        case 'A': _MWindowOnKeyDown(MKey_A); break;
        case 'B': _MWindowOnKeyDown(MKey_B); break;
        case 'C': _MWindowOnKeyDown(MKey_C); break;
        case 'D': _MWindowOnKeyDown(MKey_D); break;
        case 'E': _MWindowOnKeyDown(MKey_E); break;
        case 'F': _MWindowOnKeyDown(MKey_F); break;
        case 'G': _MWindowOnKeyDown(MKey_G); break;
        case 'H': _MWindowOnKeyDown(MKey_H); break;
        case 'I': _MWindowOnKeyDown(MKey_I); break;
        case 'J': _MWindowOnKeyDown(MKey_J); break;
        case 'K': _MWindowOnKeyDown(MKey_K); break;
        case 'L': _MWindowOnKeyDown(MKey_L); break;
        case 'M': _MWindowOnKeyDown(MKey_M); break;
        case 'N': _MWindowOnKeyDown(MKey_N); break;
        case 'O': _MWindowOnKeyDown(MKey_O); break;
        case 'P': _MWindowOnKeyDown(MKey_P); break;
        case 'Q': _MWindowOnKeyDown(MKey_Q); break;
        case 'R': _MWindowOnKeyDown(MKey_R); break;
        case 'S': _MWindowOnKeyDown(MKey_S); break;
        case 'T': _MWindowOnKeyDown(MKey_T); break;
        case 'U': _MWindowOnKeyDown(MKey_U); break;
        case 'V': _MWindowOnKeyDown(MKey_V); break;
        case 'W': _MWindowOnKeyDown(MKey_W); break;
        case 'X': _MWindowOnKeyDown(MKey_X); break;
        case 'Y': _MWindowOnKeyDown(MKey_Y); break;
        case 'Z': _MWindowOnKeyDown(MKey_Z); break;
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
        case WM_MOUSEWHEEL : return OnMouseWheel (wnd, wParam, lParam);
        case WM_COMMAND    : return OnCommand    (wnd, wParam, lParam);
        case WM_KEYDOWN    : return OnKeyDown    (wnd, wParam, lParam);

        default: return DefWindowProcW(wnd, msg, wParam, lParam);
    }
}

M_FUNC_EXPORT void MAppMain()
{
    HINSTANCE instance = GetModuleHandleW(nullptr);

    //register window class.
    LPCWSTR className = L"MWindow";
    WNDCLASSW wndClass = {0};

    wndClass.style         = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc   = WindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = instance;
    wndClass.hIcon         = nullptr;
    wndClass.hCursor       = LoadCursorW(nullptr, (LPCWSTR)IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wndClass.lpszMenuName  = nullptr;
    wndClass.lpszClassName = className;

    RegisterClassW(&wndClass);

    //show window.
    HWND wnd = CreateWindowExW(
        /* dwExStyle    */ 0,
        /* lpClassName  */ className,
        /* lpWindowName */ (const WCHAR *)MWindowTitleU16Name,
        /* dwStyle      */ WS_OVERLAPPEDWINDOW,
        /* x            */ WindowFrameX,
        /* y            */ WindowFrameY,
        /* width        */ WindowFrameWidth ,
        /* height       */ WindowFrameHeight,
        /* hWndParent   */ nullptr,
        /* hMenu        */ nullptr,
        /* hInstance    */ instance,
        /* lpParam      */ nullptr
    );

    ShowWindow(wnd, SW_SHOWDEFAULT);
    UpdateWindow(wnd);

    //message loop.
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

int APIENTRY wWinMain(
    _In_     HINSTANCE instance,
    _In_opt_ HINSTANCE prevInst,
    _In_     LPWSTR    cmdLine ,
    _In_     int       showCmd )
{
    MAppMain();
    return 0;
}
