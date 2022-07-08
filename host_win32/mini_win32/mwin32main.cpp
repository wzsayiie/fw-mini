#include <clocale>
#include <io.h>
#include <windowsx.h>
#include "mapp.h"
#include "mcontext.h"
#include "mencode.h"
#include "mpcbundle.h"
#include "mwin32imagefactory.h"
#include "mwin32jsrt.h"
#include "mwin32paint.h"
#include "mwindow.h"

const POINT    ConsoleOrigin       = {   20, 100 };
const POINT    WindowOrigin        = { 1000, 100 };
const SIZE     WindowSize          = {  376, 679 };
const UINT_PTR AppUpdateTimerId    = 1;
const UINT_PTR WindowUpdateTimerId = 2;

static HWND    sEditWnd         = nullptr;
static WNDPROC sEditDefaultProc = nullptr;
static bool    sLButtonDowned   = false;
static bool    sEditEmitEnter   = false;

static void OpenConsole(void)
{
    //is there a console?
    if (_isatty(_fileno(stdout)))
    {
        return;
    }

    AllocConsole();
    {
        FILE *newStdin  = nullptr;
        FILE *newStdout = nullptr;
        FILE *newStderr = nullptr;
        _wfreopen_s(&newStdin , L"conin$" , L"r", stdin );
        _wfreopen_s(&newStdout, L"conout$", L"w", stdout);
        _wfreopen_s(&newStderr, L"conout$", L"w", stderr);

        //IMPORTANT: use local character set.
        setlocale(LC_CTYPE, "");
    }
    HWND wnd = nullptr;
    {
        GetConsoleTitleW(dash::buffer<WCHAR>(), (DWORD)dash::buffer_size<WCHAR>());
        wnd = FindWindowW(nullptr, dash::buffer<WCHAR>());
    }

    //move the console window.
    SetWindowPos(wnd, HWND_TOP, ConsoleOrigin.x, ConsoleOrigin.y, 0, 0, SWP_NOSIZE);

    //disable the close button of console window.
    //clicking it will cause the program to exit abnormally.
    HMENU menu = GetSystemMenu(wnd, FALSE);
    EnableMenuItem(menu, SC_CLOSE, MF_DISABLED);
}

static SIZE GetClientSize(HWND wnd)
{
    RECT rect = {0};
    GetClientRect(wnd, &rect);

    SIZE size = {0};
    size.cx = rect.right  - rect.left;
    size.cy = rect.bottom - rect.top ;

    return size;
}

static LRESULT CALLBACK EditCustomProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KEYDOWN && wParam == VK_RETURN)
    {
        sEditEmitEnter = true;

        auto wParam = MAKEWPARAM(0, EN_CHANGE);
        auto lParam = (LPARAM)wnd;
        SendMessageW(GetParent(wnd), WM_COMMAND, wParam, lParam);
        
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

static void UpdateEditFrameWithSpace(int spaceWidth, int spaceHeight)
{
    const int EditWidth  = 300;
    const int EditHeight =  20;

    SetWindowPos(
        /* wnd    */ sEditWnd,
        /* insert */ nullptr,
        /* x      */ (spaceWidth  - EditWidth ) / 2,
        /* y      */ (spaceHeight - EditHeight) / 2,
        /* width  */ EditWidth,
        /* height */ EditHeight,
        /* flags  */ 0
    );
}

static void UpdateEditState()
{
    MWindow *window = MWindow::mainWindow();
    if (!window->checkWritingUpdated())
    {
        return;
    }

    if (window->writingEnabled())
    {
        std::string    u8text  = window->checkWritingRawText();
        std::u16string u16text = MU16StringFromU8(u8text.c_str());

        auto bgn = (WPARAM)0;
        auto end = (LPARAM)u16text.size();
        SetWindowTextW(sEditWnd, (LPWSTR)u16text.c_str());
        SendMessageW(sEditWnd, EM_SETSEL, bgn, end);

        ShowWindow(sEditWnd, SW_SHOW);
        SetFocus(sEditWnd);
    }
    else
    {
        ShowWindow(sEditWnd, SW_HIDE);
    }
}

static void OnCreate(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    SIZE clientSize = GetClientSize(wnd);

    //components initialization.
    OpenConsole();
    MWin32GdiplusStartup();
    MPCBundle::install();
    MWin32ImageFactory::install();
    MWin32JsVM::install();

    //create edit.
    CreateEditWithParent(wnd);
    UpdateEditFrameWithSpace((int)clientSize.cx, (int)clientSize.cy);

    //application events:
    MApp::instance()->launch();

    SetTimer(wnd, AppUpdateTimerId, (UINT)(1000 * MApp::UpdateEverySeconds), nullptr);

    //window events:
    MWindow *window = MWindow::mainWindow();
    window->resizePixel((float)clientSize.cx, (float)clientSize.cy);
    window->load();

    SetTimer(wnd, WindowUpdateTimerId, (UINT)(1000 * MWindow::UpdateEverySeconds), nullptr);
}

static void OnShowWindow(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MWindow::mainWindow();

    auto shown = (BOOL)wParam;
    if (shown)
    {
        window->show();
    }
    else
    {
        window->hide();
    }
}

static void OnDestroy(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    KillTimer(wnd, WindowUpdateTimerId);
    KillTimer(wnd, AppUpdateTimerId);
    MWin32GdiplusShutdown();

    PostQuitMessage(0);
}

static void OnTimer(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    auto timerId = (UINT_PTR)wParam;
    if (timerId == AppUpdateTimerId)
    {
        MApp::instance()->update();
    }
    else if (timerId == WindowUpdateTimerId)
    {
        UpdateEditState();
        InvalidateRect(wnd, nullptr, FALSE);
    }
}

static void OnSize(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    auto clientSize = lParam;
    int  width      = LOWORD(clientSize);
    int  height     = HIWORD(clientSize);

    UpdateEditFrameWithSpace(width, height);
    MWindow::mainWindow()->resizePixel((float)width, (float)height);
}

static void OnPaint(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    SIZE clientSize = GetClientSize(wnd);

    PAINTSTRUCT paint = {0};
    BeginPaint(wnd, &paint);
    {
        HBITMAP bmp = CreateCompatibleBitmap(paint.hdc, clientSize.cx, clientSize.cy);
        HDC dc = CreateCompatibleDC(paint.hdc);
        SelectBitmap(dc, bmp);

        //set the background gray.
        RECT rect = { 0, 0, clientSize.cx, clientSize.cy };
        HBRUSH backgroundBrush = CreateSolidBrush(0xF0F0F0);
        FillRect(dc, &rect, backgroundBrush);
        DeleteBrush(backgroundBrush);

        MWindow::mainWindow()->draw();
        MWin32Paint(dc);
        MContextReset();

        BitBlt(paint.hdc, 0, 0, clientSize.cx, clientSize.cy, dc, 0, 0, SRCCOPY);
        DeleteDC(dc);
        DeleteBitmap(bmp);
    }
    EndPaint(wnd, &paint);
}

static void OnLButtonDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MWindow::mainWindow();

    LPARAM clientPoint = lParam;
    auto x = GET_X_LPARAM(clientPoint);
    auto y = GET_Y_LPARAM(clientPoint);

    //mouse event.
    window->mouseMovePixel((float)x, (float)y);

    //touch event.
    sLButtonDowned = true;
    window->touchBeginPixel((float)x, (float)y);
    //NOTE: to capture events when the mouse moves outside the window.
    SetCapture(wnd);
}

static void OnMouseMove(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MWindow::mainWindow();

    LPARAM clientPoint = lParam;
    int x = GET_X_LPARAM(clientPoint);
    int y = GET_Y_LPARAM(clientPoint);

    //mouse event:
    SIZE clientSize = GetClientSize(wnd);

    if (0 <= x && x <= clientSize.cx
     && 0 <= y && y <= clientSize.cy)
    {
        window->mouseMovePixel((float)x, (float)y);
    }

    //touch event.
    if (sLButtonDowned)
    {
        window->touchMovePixel((float)x, (float)y);
    }
}

static void OnLButtonUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MWindow::mainWindow();

    LPARAM clientPoint = lParam;
    int x = GET_X_LPARAM(clientPoint);
    int y = GET_Y_LPARAM(clientPoint);

    //mouse event:
    SIZE clientSize = GetClientSize(wnd);

    if (0 <= x && x <= clientSize.cx
     && 0 <= y && y <= clientSize.cy)
    {
        window->mouseMovePixel((float)x, (float)y);
    }

    //touch event.
    window->touchEndPixel((float)x, (float)y);
    sLButtonDowned = false;
    ReleaseCapture();
}

static void OnMouseWheel(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    short delta = GET_WHEEL_DELTA_WPARAM(wParam);
    MWindow::mainWindow()->mouseWheel(delta);
}

static void OnKeyDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MWindow::mainWindow();

    WPARAM virtualkey = wParam;
    switch (virtualkey)
    {
        case VK_BACK  : window->key(MKey::Back ); break;
        case VK_RETURN: window->key(MKey::Enter); break;
        case VK_SPACE : window->key(MKey::Space); break;

        case VK_LEFT  : window->key(MKey::Left ); break;
        case VK_UP    : window->key(MKey::Up   ); break;
        case VK_RIGHT : window->key(MKey::Right); break;
        case VK_DOWN  : window->key(MKey::Down ); break;

        case 'A'      : window->key(MKey::A    ); break;
        case 'D'      : window->key(MKey::D    ); break;
        case 'S'      : window->key(MKey::S    ); break;
        case 'W'      : window->key(MKey::W    ); break;
    }
}

static void OnCommand(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    int  notifyCode = HIWORD(wParam);
    HWND controlWnd = (HWND)(lParam);

    if (notifyCode == EN_CHANGE && controlWnd == sEditWnd)
    {
        GetWindowTextW(sEditWnd, dash::buffer<WCHAR>(), dash::buffer_size<WCHAR>());
        std::string u8text = MU8StringFromU16(dash::buffer<char16_t>());

        if (sEditEmitEnter)
        {
            sEditEmitEnter = false;
            MWindow::mainWindow()->write(u8text, true);
        }
        else
        {
            MWindow::mainWindow()->write(u8text, false);
        }
    }
}

static LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE     : OnCreate     (wnd, wParam, lParam); return 0;
        case WM_SHOWWINDOW : OnShowWindow (wnd, wParam, lParam); return 0;
        case WM_DESTROY    : OnDestroy    (wnd, wParam, lParam); return 0;
        case WM_TIMER      : OnTimer      (wnd, wParam, lParam); return 0;
        case WM_SIZE       : OnSize       (wnd, wParam, lParam); return 0;
        case WM_PAINT      : OnPaint      (wnd, wParam, lParam); return 0;
        case WM_LBUTTONDOWN: OnLButtonDown(wnd, wParam, lParam); return 0;
        case WM_MOUSEMOVE  : OnMouseMove  (wnd, wParam, lParam); return 0;
        case WM_LBUTTONUP  : OnLButtonUp  (wnd, wParam, lParam); return 0;
        case WM_MOUSEWHEEL : OnMouseWheel (wnd, wParam, lParam); return 0;
        case WM_KEYDOWN    : OnKeyDown    (wnd, wParam, lParam); return 0;
        case WM_COMMAND    : OnCommand    (wnd, wParam, lParam); return 0;

        default: return DefWindowProcW(wnd, msg, wParam, lParam);
    }
}

extern "C" d_exportable void MAppMain()
{
    HINSTANCE instance = GetModuleHandleW(nullptr);

    //register window class:
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

    //show window:
    std::u16string title = MU16StringFromU8(MWindow::TitleName);
    HWND wnd = CreateWindowExW(
        /* dwExStyle    */ 0,
        /* lpClassName  */ className,
        /* lpWindowName */ (LPCWSTR)title.c_str(),
        /* dwStyle      */ WS_OVERLAPPEDWINDOW,
        /* x            */ WindowOrigin.x,
        /* y            */ WindowOrigin.y,
        /* width        */ WindowSize.cx,
        /* height       */ WindowSize.cy,
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
