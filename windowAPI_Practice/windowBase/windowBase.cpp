#include"windowBase.h"

LRESULT windowBase::window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_NCDESTROY: {
        auto self = reinterpret_cast<windowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (self) {
            if (callback_) { callback_(); }
            SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        }
        return 0;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

[[nodiscard]] HWND windowBase::get_hwnd()const noexcept {
	return hwnd_;
}