
#include"Application.h"

INT_PTR CALLBACK Application::About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hWnd, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

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