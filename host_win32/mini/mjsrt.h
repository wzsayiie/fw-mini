#pragma once

#include <windows.h>

const UINT WM_USER_JSTASK = WM_USER + 'J';

void MInstallJSRuntime(HWND wnd);
void MConsumeJSTasks();
