#pragma once

#include"framework.h"
#include "windowAPI.h"
#include<objbase.h>

#include<vector>
#include<concepts>
#include<memory>
#include"ApplocationExtensionBase.h"
#include"Extension/SelectFileExtension.h"

#define MAX_LOADSTRING 100

class Application final
{
public:

    Application(HINSTANCE hInstance) :
        hInstance_{ hInstance } {

        HRESULT hr = CoInitializeEx(
            nullptr,
            COINIT_APARTMENTTHREADED
        );

        if (SUCCEEDED(hr)) {
            CoUninitialize();
        }
    }

    ~Application() = default;

    int ApplyApp(int nCmdShow) {

        AppRegisterClass();

        if (!InitInstance(nCmdShow)) {
            return FALSE;
        }

        HACCEL hAccelTable = LoadAccelerators(hInstance_, MAKEINTRESOURCE(IDC_WINDOWAPI));

        MSG msg;

        while (GetMessageW(&msg, nullptr, 0, 0) > 0)
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        return (int)msg.wParam;
    }

private:

	HINSTANCE hInstance_{};

    WCHAR Title_[MAX_LOADSTRING] = L"WindowAPI";
    WCHAR WindowClass_[MAX_LOADSTRING] = L"WindowAPI";

    std::vector<std::unique_ptr<ApplicationExtensionBase>> ExtensionArray{};

    LRESULT static CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

        /* ===== 初期化時 ===== */
        if (message == WM_NCCREATE) {

            auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            auto pWnd = reinterpret_cast<Application*>(cs->lpCreateParams);

            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

            return TRUE;
        }

        if (auto pWnd = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)); pWnd) {

            /* ===== 最終破棄時 ===== */
            if (message == WM_NCDESTROY) {
                SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
            }
            return pWnd->AppProc(hWnd, message, wParam, lParam);
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }


    /// <summary>
    /// アプリケーションプロシージャ
    /// </summary>
    /// <param name="hWnd"></param>
    /// <param name="message"></param>
    /// <param name="wParam"></param>
    /// <param name="lParam"></param>
    /// <returns></returns>
    LRESULT AppProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

        if (!ExtensionArray.empty()) {
            for (auto& ex : ExtensionArray) {
                ex->ExtensionProc(hWnd, message, wParam, lParam);
            }
        }
        switch (message)
        {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInstance_, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                CheckCloseApp(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_KEYDOWN:
            Keybord(hWnd, message, wParam, lParam);
            break;
        case WM_KEYUP:
            Keybord(hWnd, message, wParam, lParam);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    INT_PTR static CALLBACK About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    void Keybord(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    bool SelectYesNo(LPCWSTR mainText , LPCWSTR titleText);

    void CheckCloseApp(HWND hWnd) {

        if (SelectYesNo(L"Close App ?", L"")) {
            DestroyWindow(hWnd);
        }
    }

    /* ===== 初期化関数 ===== */

    ATOM AppRegisterClass() {

        WNDCLASSEXW wcex{};

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance_;
        wcex.hIcon = LoadIcon(hInstance_, MAKEINTRESOURCE(IDI_WINDOWAPI));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWAPI);
        wcex.lpszClassName = WindowClass_;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&wcex);
    }

    BOOL InitInstance(int nCmdShow)
    {

        HWND hWnd = CreateWindowW(WindowClass_, Title_, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, nullptr, nullptr, hInstance_, this);

        if (!hWnd) {
            return FALSE;
        }

        //  タイトルを明示的に設定
        SetWindowTextW(hWnd, Title_);

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        ExtensionArray.push_back(
            std::make_unique<SelectFileExtension::SelectFileExtension>(
                hWnd, 
                hInstance_,
                SelectFileExtension::FileExtensionFlags::mp3
            )
        );
        
        return TRUE;
    }

};
