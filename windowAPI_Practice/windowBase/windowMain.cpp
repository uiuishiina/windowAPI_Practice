#include"windowMain.h"

LRESULT windowMain::window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
[[nodiscard]] bool windowMain::create_window(HINSTANCE hInstance, WNDCLASSEX ex) {

    hwnd_ = CreateWindowEx(
        0,                        //    拡張スタイル
        ex.lpszClassName,        //    クラス名
        ex.lpszClassName,        //    ウィンドウタイトル
        WS_OVERLAPPEDWINDOW,    //    ウィンドウスタイル
        CW_USEDEFAULT, CW_USEDEFAULT, //    位置とサイズ
        1280, 720,
        nullptr,                //    親ウィンドウ
        nullptr,                //    メニューハンドル
        hInstance,                //    インスタンスハンドル
        this                    //    追加パラメータ
    );

    if (hwnd_) {
        ShowWindow(hwnd_, SW_SHOW);
    }

	return true;
}