#include"windowBase.h"

LRESULT windowBase::window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_NCDESTROY:
        if (ondestroy_callback) { ondestroy_callback(); }
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

[[nodiscard]] HWND windowBase::get_hwnd() const noexcept {
    return hwnd_;
}