#include <clocale>
#include <io.h>
#include <windows.h>
#include <windowsx.h>
#include "minikit.h"

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

static SIZE GetClientSize(HWND wnd)
{
    RECT rect = {0};
    GetClientRect(wnd, &rect);

    SIZE size = {0};
    size.cx = rect.right  - rect.left;
    size.cy = rect.bottom - rect.top ;

    return size;
}

static std::u16string CopyEditText(HWND edit)
{
    WCHAR buffer[1024] = L"";
    GetWindowTextW(edit, buffer, (int)(sizeof(buffer) / sizeof(*buffer)));

    return (const char16_t *)buffer;
}

const UINT_PTR AppUpdateTimerId    = 1;
const UINT_PTR WindowUpdateTimerId = 2;

static HWND    sEditWnd         = nullptr;
static WNDPROC sEditDefaultProc = nullptr;
static bool    sLButtonDowned   = false;

static LRESULT CALLBACK EditCustomProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_KEYDOWN && wParam == VK_RETURN)
    {
        std::u16string u16text = CopyEditText(wnd);
        std::string    u8text  = MU8StringFromU16(u16text.c_str());

        MGetMainWindow()->write(u8text, true);

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
    MWindow *window = MGetMainWindow();
    if (!window->checkWritingUpdated())
    {
        return;
    }

    if (window->writingEnabled())
    {
        std::string    u8text  = window->checkWritingRawText();
        std::u16string u16text = MU16StringFromU8(u8text.c_str());

        auto selectionBegin = (WPARAM)0;
        auto selectionEnd   = (LPARAM)u16text.size();
        SetWindowTextW(sEditWnd, (LPWSTR)u16text.c_str());
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
    //MPaintStart();

    SIZE clientSize = GetClientSize(wnd);

    //create edit control.
    CreateEditWithParent(wnd);
    AdjustEditPosition(clientSize.cx, clientSize.cy);

    //application events.
    MGetApp()->launch();
    SetTimer(wnd, AppUpdateTimerId, (UINT)(1000 * MAppUpdateEverySeconds), nullptr);

    //window events:
    MWindow *window = MGetMainWindow();
    window->resizePixel((float)clientSize.cx, (float)clientSize.cy);
    window->load();

    SetTimer(wnd, WindowUpdateTimerId, (UINT)(1000 * MWindowUpdateEverySeconds), nullptr);

    return 0;
}

static LRESULT OnShowWindow(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MGetMainWindow();

    auto shown = (BOOL)wParam;
    if (shown)
    {
        window->show();
    }
    else
    {
        window->hide();
    }

    return 0;
}

static LRESULT OnDestroy(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    //MPaintStop();

    KillTimer(wnd, WindowUpdateTimerId);
    KillTimer(wnd, AppUpdateTimerId);

    PostQuitMessage(0);
    return 0;
}

static LRESULT OnTimer(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    UINT_PTR timerId = wParam;
    if (timerId == AppUpdateTimerId)
    {
        UpdateEditState();
        MGetApp()->update();
    }
    else if (timerId == WindowUpdateTimerId)
    {
        InvalidateRect(wnd, nullptr, FALSE);
    }

    return 0;
}

static LRESULT OnSize(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MGetMainWindow();

    LPARAM clientSize = lParam;
    int width  = LOWORD(clientSize);
    int height = HIWORD(clientSize);

    AdjustEditPosition(width, height);
    window->resizePixel((float)width, (float)height);
    
    return 0;
}

static LRESULT OnPaint(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    SIZE clientSize = GetClientSize(wnd);

    PAINTSTRUCT paint = {0};
    BeginPaint(wnd, &paint);
    {
        HDC dc = CreateCompatibleDC(paint.hdc);
        HBITMAP bmp = CreateCompatibleBitmap(paint.hdc, clientSize.cx, clientSize.cy);
        SelectBitmap(dc, bmp);

        //set the background gray.
        RECT rect = { 0, 0, clientSize.cy, clientSize.cy };
        HBRUSH backgroundBrush = CreateSolidBrush(0xF0F0F0);
        FillRect(dc, &rect, backgroundBrush);
        DeleteBrush(backgroundBrush);

        //MPaint(dc);

        BitBlt(paint.hdc, 0, 0, clientSize.cx, clientSize.cy, dc, 0, 0, SRCCOPY);
        DeleteDC(dc);
        DeleteBitmap(bmp);
    }
    EndPaint(wnd, &paint);
    return 0;
}

static LRESULT OnLButtonDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MGetMainWindow();

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

    return 0;
}

static LRESULT OnMouseMove(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MGetMainWindow();

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

    return 0;
}

static LRESULT OnLButtonUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MGetMainWindow();

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

    return 0;
}

static LRESULT OnMouseWheel(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    short delta = GET_WHEEL_DELTA_WPARAM(wParam);
    MGetMainWindow()->mouseWheel(delta);

    return 0;
}

static LRESULT OnKeyDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MWindow *window = MGetMainWindow();

    WPARAM virtualkey = wParam;
    switch (virtualkey)
    {
        case VK_BACK  : window->keyDown(MKey::Back ); break;
        case VK_RETURN: window->keyDown(MKey::Enter); break;
        case VK_SPACE : window->keyDown(MKey::Space); break;
        case VK_LEFT  : window->keyDown(MKey::Left ); break;
        case VK_UP    : window->keyDown(MKey::Up   ); break;
        case VK_RIGHT : window->keyDown(MKey::Right); break;
        case VK_DOWN  : window->keyDown(MKey::Down ); break;
        case 'A'      : window->keyDown(MKey::A    ); break;
        case 'D'      : window->keyDown(MKey::D    ); break;
        case 'S'      : window->keyDown(MKey::S    ); break;
        case 'W'      : window->keyDown(MKey::W    ); break;
    }
    return 0;
}

static LRESULT OnCommand(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    HWND controlWnd = (HWND)(lParam);
    int  notifyCode = HIWORD(wParam);

    if (controlWnd == sEditWnd && notifyCode == EN_CHANGE)
    {
        std::u16string u16text = CopyEditText(sEditWnd);
        std::string    u8text  = MU8StringFromU16(u16text.c_str());

        MGetMainWindow()->write(u8text, false);
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
        case WM_KEYDOWN    : return OnKeyDown    (wnd, wParam, lParam);
        case WM_COMMAND    : return OnCommand    (wnd, wParam, lParam);

        default: return DefWindowProcW(wnd, msg, wParam, lParam);
    }
}

extern "C" __declspec(dllexport) void MAppMain()
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
    HWND wnd = CreateWindowExW(
        /* dwExStyle    */ 0,
        /* lpClassName  */ className,
        /* lpWindowName */ L"Mini",
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
