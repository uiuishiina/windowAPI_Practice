#include"windowSub.h"

[[nodiscard]] bool windowSub::create_window(HINSTANCE hInstance, std::wstring& class_name) {
    hwnd_ = CreateWindowEx(
        0,
        class_name.c_str(),
        class_name.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        960, 540,
        nullptr,
        nullptr,
        hInstance,
        this
    );

    if (!hwnd_) {
        return false;
    }

    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
    return true;
}