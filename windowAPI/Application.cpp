
#include"Application.h"

void Application::Keybord(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (message == WM_KEYDOWN) {
        bool repeat = (lParam & (1 << 30)) != 0;
        if (repeat) {
            return;
        }
        switch (wParam)
        {
        case VK_ESCAPE:
            CheckCloseApp(hWnd);
            break;
        case VK_LEFT:
            break;
        case VK_RIGHT:
            break;
        default:
            break;
        }
        return;
    }
    if (message == WM_KEYUP) {
        bool repeat = (lParam & (1 << 30)) != 0;
        if (repeat) {
            return;
        }
        switch (wParam)
        {
        default:
            break;
        }
        return;
    }
}

bool Application::SelectYesNo(LPCWSTR mainText, LPCWSTR titleText) {

    int result = MessageBoxW(
        nullptr,
        mainText,
        titleText,
        MB_YESNO
    );

    return result == IDYES ? true : false;
}