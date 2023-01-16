#include <clocale>
#include <io.h>
#include <windowsx.h>
#include "dbuffer.h"
#include "dfile.h"
#include "mapp.h"
#include "mcontext.h"
#include "mencode.h"
#include "mpcbundle.h"
#include "mwin32gdiplus.h"
#include "mwin32imagefactory.h"
#include "mwin32jsrt.h"
#include "mwin32paint.h"
#include "mwindow.h"

const char *const PlacementFileName = "window_placement";

const UINT_PTR AppUpdateTimerId    = 1;
const UINT_PTR WindowUpdateTimerId = 2;

const  int     EditMaxWidth     = 300;
const  int     EditHeight       =  20;
static HWND    sEditWnd         = nullptr;
static WNDPROC sEditDefaultProc = nullptr;
static bool    sLButtonDowned   = false;

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
    SetWindowPos(wnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE);

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

static MKbModifiers GetModifiers()
{
    MKbModifiers modifiers = 0;

    //high byte flags 'pressed'.
    if (HIBYTE(GetKeyState(VK_MENU   ))) { modifiers |= MKbModifier_Alt  ; }
    if (HIBYTE(GetKeyState(VK_CONTROL))) { modifiers |= MKbModifier_Ctrl ; }
    if (HIBYTE(GetKeyState(VK_SHIFT  ))) { modifiers |= MKbModifier_Shift; }
    //low byte flags 'toggled'.
    if (LOBYTE(GetKeyState(VK_CAPITAL))) { modifiers |= MKbModifier_Caps ; }

    return modifiers;
}

static LRESULT CALLBACK EditCustomProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //custom process for "tab" and "enter" keys. application may relies them to move focus.
    MKbKeyCode code = MKbKeyCode::Null;
    if (msg == WM_KEYDOWN)
    {
        switch (wParam)
        {
            case VK_TAB   : code = MKbKeyCode::Tab  ; break;
            case VK_RETURN: code = MKbKeyCode::Enter; break;
        }
    }

    if (code != MKbKeyCode::Null)
    {
        auto evt = MKbKey::make(code, GetModifiers());
        MWindow::mainWindow()->kbKey(evt);
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
    int width = spaceWidth > EditMaxWidth ? EditMaxWidth : spaceWidth;

    SetWindowPos(
        /* wnd    */ sEditWnd,
        /* insert */ nullptr,
        /* x      */ (spaceWidth  - width     ) / 2,
        /* y      */ (spaceHeight - EditHeight) / 2,
        /* width  */ width,
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

static std::string GetPlacementFilePath() {
    std::string path = MBundle::instance()->temporaryDirectory();
    return path + "\\" + PlacementFileName;
}

static void RestorePlacement(HWND wnd) {
    std::string path = GetPlacementFilePath();
    if (!MFileManager::instance()->fileExistsAt(path)) {
        return;
    }

    //read.
    WINDOWPLACEMENT placement = { (UINT)sizeof(WINDOWPLACEMENT) };
    dash::read_file(path.c_str(), [&](int) { return &placement; });

    //set.
    SetWindowPlacement(wnd, &placement);
}

static void SavePlacement(HWND wnd) {
    //get.
    WINDOWPLACEMENT placement = { (UINT)sizeof(WINDOWPLACEMENT) };
    GetWindowPlacement(wnd, &placement);

    //write.
    std::string path = GetPlacementFilePath();
    dash::write_file(path.c_str(), &placement, (int)sizeof(placement));
}

static void OnCreate(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    //components initialization.
    OpenConsole();
    MWin32GdiplusStartup();
    MPCBundle::install();
    MWin32ImageFactory::install();
    MWin32JsVM::install();

    //window configuration:
    RestorePlacement(wnd);

    SIZE clientSize = GetClientSize(wnd);

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

    //IMPORTANT: emit a "hide" event.
    //ensuring the lifecycle order of "load", to "show", to "hide".
    MWindow *window = MWindow::mainWindow();
    window->hide();

    SavePlacement(wnd);
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
    auto x = (float)GET_X_LPARAM(lParam);
    auto y = (float)GET_Y_LPARAM(lParam);

    //NOTE:
    //mouse event.
    auto moveEvent = MMouseMove::makePixel(x, y);
    MWindow::mainWindow()->mouseMove(moveEvent);

    //touch event:
    auto touchEvent = MTouch::makeBeginPixel(x, y, MTouchSource::LButton);
    auto kbKeyEvent = MKbKey::make(MKbKeyCode::Null, GetModifiers());
    MWindow::mainWindow()->touch(touchEvent, kbKeyEvent);

    sLButtonDowned = true;
    //NOTE: to capture events when the mouse moves outside the window.
    SetCapture(wnd);
}

static void OnMouseMove(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    auto x = (float)GET_X_LPARAM(lParam);
    auto y = (float)GET_Y_LPARAM(lParam);

    //mouse event:
    SIZE clientSize = GetClientSize(wnd);

    if (0 <= x && x <= clientSize.cx
     && 0 <= y && y <= clientSize.cy)
    {
        auto moveEvent = MMouseMove::makePixel(x, y);
        MWindow::mainWindow()->mouseMove(moveEvent);
    }

    //touch event.
    if (sLButtonDowned)
    {
        auto touchEvent = MTouch::makeMovePixel(x, y, MTouchSource::LButton);
        MWindow::mainWindow()->touch(touchEvent, nullptr);
    }
}

static void OnLButtonUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    auto x = (float)GET_X_LPARAM(lParam);
    auto y = (float)GET_Y_LPARAM(lParam);

    //mouse event:
    SIZE clientSize = GetClientSize(wnd);

    if (0 <= x && x <= clientSize.cx
     && 0 <= y && y <= clientSize.cy)
    {
        auto moveEvent = MMouseMove::makePixel(x, y);
        MWindow::mainWindow()->mouseMove(moveEvent);
    }

    //touch event:
    auto touchEvent = MTouch::makeEndPixel(x, y, MTouchSource::LButton);
    MWindow::mainWindow()->touch(touchEvent, nullptr);

    sLButtonDowned = false;
    ReleaseCapture();
}

static void OnMouseWheel(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    auto delta = (float)GET_WHEEL_DELTA_WPARAM(wParam);
    auto x     = (float)GET_X_LPARAM(lParam);
    auto y     = (float)GET_Y_LPARAM(lParam);
    
    auto evt = MMouseWheel::makePixel(x, y, delta);
    MWindow::mainWindow()->mouseWheel(evt);
}

static void OnKeyDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    MKbKeyCode code = MKbKeyCode::Null;
    switch (wParam)
    {
        case VK_BACK  : code = MKbKeyCode::Back ; break;
        case VK_TAB   : code = MKbKeyCode::Tab  ; break;
        case VK_RETURN: code = MKbKeyCode::Enter; break;
        case VK_SPACE : code = MKbKeyCode::Space; break;

        case VK_LEFT  : code = MKbKeyCode::Left ; break;
        case VK_UP    : code = MKbKeyCode::Up   ; break;
        case VK_RIGHT : code = MKbKeyCode::Right; break;
        case VK_DOWN  : code = MKbKeyCode::Down ; break;

        case 'A'      : code = MKbKeyCode::A    ; break;
        case 'D'      : code = MKbKeyCode::D    ; break;
        case 'S'      : code = MKbKeyCode::S    ; break;
        case 'W'      : code = MKbKeyCode::W    ; break;

        //unsupported keys.
        default: return;
    }

    auto evt = MKbKey::make(code, GetModifiers());
    MWindow::mainWindow()->kbKey(evt);
}

static void OnCommand(HWND wnd, WPARAM wParam, LPARAM lParam)
{
    int  notifyCode = HIWORD(wParam);
    HWND controlWnd = (HWND)(lParam);

    if (notifyCode == EN_CHANGE && controlWnd == sEditWnd)
    {
        GetWindowTextW(sEditWnd, dash::buffer<WCHAR>(), dash::buffer_size<WCHAR>());
        std::string u8text = MU8StringFromU16(dash::buffer<char16_t>());

        auto evt = MWriting::make(u8text);
        MWindow::mainWindow()->writing(evt);
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
        case WM_SYSKEYDOWN : OnKeyDown    (wnd, wParam, lParam); return 0;
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
        /* x            */ CW_USEDEFAULT,
        /* y            */ CW_USEDEFAULT,
        /* width        */ CW_USEDEFAULT,
        /* height       */ CW_USEDEFAULT,
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
