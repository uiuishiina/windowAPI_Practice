#include"windowManager.h"
#include"../windowBase/windowMain.h"
#include"../windowBase/windowSub.h"

namespace {
    const wchar_t* main_name = L"main_window";
    const wchar_t* sub_name = L"sub_window";
}

//---------------------------------------------------------------------------------------------------

LRESULT CALLBACK windowManager::StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCCREATE) {
        auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        auto pWnd = reinterpret_cast<windowBase*>(cs->lpCreateParams);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        pWnd->hwnd_ = hwnd;

        return true;
    }

    auto pWnd = reinterpret_cast<windowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (pWnd) {
        return pWnd->window_proc(hwnd, msg, wParam, lParam);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//---------------------------------------------------------------------------------------------------

[[nodiscard]] bool windowManager::initalize(HINSTANCE hInstance) {
	register_ = std::make_unique<windowRegister>();
    hInstance_ = hInstance;
    return true;
}

//---------------------------------------------------------------------------------------------------

[[nodiscard]] bool windowManager::create_window(windowclassID id) {

    auto window_class = register_->get_class(id);
    if (!window_class.has_value()) {
        register_submit(id);
        window_class = register_->get_class(id);
    }

    std::unique_ptr<windowBase> base{};

    switch (id)
    {
    case windowclassID::Main:
        base = std::make_unique<windowMain>();
        break;
    case windowclassID::Sub:
        base = std::make_unique<windowSub>();
        break;
    default:
        return false;
        break;
    }
    if (!base->create_window(hInstance_, window_class.value())) {
        return false;
    }
    auto hwnd = base->get_hwnd();

    base->callback_ = [this, hwnd]() {
        this->ondestroy_window(hwnd);
        };

    sub_window.emplace(hwnd,std::move(base));

    return true;
}

//---------------------------------------------------------------------------------------------------
//
//void windowManager::attach_callback(windowBase& wnd) {
//    auto hwnd = wnd.hwnd_;
//    wnd.callback_ = [this, hwnd]() {
//        this->ondestroy_window(hwnd);
//        };
//}
//---------------------------------------------------------------------------------------------------

void windowManager::register_submit(windowclassID id) {

    const wchar_t* name{};

    switch (id)
    {
    case windowclassID::Main:
        name = main_name;
        break;
    case windowclassID::Sub:
        name = sub_name;
        break;
    default:
        break;
    }

    WNDCLASSEX wc;
    std::memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = StaticWindowProc;
    wc.hInstance = hInstance_;
    wc.lpszClassName = name;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    register_->register_class(id, wc);
}

//---------------------------------------------------------------------------------------------------

[[nodiscard]] bool windowManager::message_loop() {
    MSG msg_{};
    while (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE)) {
        if (msg_.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg_);
        DispatchMessage(&msg_);
    }
    return true;
}

//---------------------------------------------------------------------------------------------------

void windowManager::ondestroy_window(HWND hwnd) {
    auto it = sub_window.find(hwnd);
    if (it == sub_window.end()) {
        return;
    }
    OutputDebugStringA("CallonDestroy\n");
    sub_window.erase(it);
}

//---------------------------------------------------------------------------------------------------

void windowManager::end() {

}