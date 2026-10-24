#pragma once

#include"ApplocationExtensionBase.h"

#include<concepts>
#include<string>
#include<vector>

namespace VideoPlaybackExtension {

    enum class videoFlags {
        mp3 = 1 << 0,
        wav = 1 << 1,
        mp4 = 1 << 2,
    };

    template<typename E>
    concept VideoFlags = std::same_as<E, videoFlags>;

    template<VideoFlags... flags>
    unsigned int CreateVideoFlag(flags... flag) {
        return (0u | ... | static_cast<unsigned int>(flag));
    }

    /// <summary>
    /// 動画再生拡張機能
    /// </summary>
    class VideoPlaybackExtension final : public ApplicationExtensionBase
    {
        WCHAR ErrorLogW[29] = L"VideoPlaybackExtension Error";
        CHAR ErrorLogA[29] = "VideoPlaybackExtension Error";
        WCHAR ListWindowClass_[16] = L"VideoListWindow";

    public:
        template<VideoFlags... flags>
        explicit VideoPlaybackExtension(HWND hWnd, HINSTANCE hInstance,flags... flag) :
            ApplicationExtensionBase(hWnd, hInstance),
            Flag_{ CreateVideoFlag(flag...) } {

            WNDCLASSEXW wc{};
            wc.cbSize = sizeof(wc);
            wc.lpfnWndProc = VideoWindowProc;
            wc.hInstance = hInstance_;
            wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            wc.hbrBackground = reinterpret_cast<HBRUSH>(
                COLOR_WINDOW + 1
                );
            wc.lpszClassName = ListWindowClass_;

            RegisterClassExW(&wc);
        }


        void ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
        
        LRESULT VideoListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

        std::wstring SelectPath() const;

        void LoadFolderToPath(const std::wstring& path);

        void OpenFile(const std::wstring& path) const;

        void ClearContainer();

        void ShowConrainer();

        bool CreateListWindow();

    private:

        unsigned int Flag_{};

        HWND ListWindow_{};
        HWND ListBox_{};

        std::vector<std::wstring> VideoPassContainer_{};

        LRESULT static CALLBACK VideoWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

            /* ===== 初期化時 ===== */
            if (message == WM_NCCREATE) {

                auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                auto pWnd = reinterpret_cast<VideoPlaybackExtension*>(cs->lpCreateParams);

                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

                return TRUE;
            }

            if (auto pWnd = reinterpret_cast<VideoPlaybackExtension*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)); pWnd) {

                /* ===== 最終破棄時 ===== */
                if (message == WM_NCDESTROY) {
                    SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
                }
                return pWnd->VideoListProc(hWnd, message, wParam, lParam);
            }

            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    };

    inline bool VideoPlaybackExtension::CreateListWindow() {
        ListWindow_ = CreateWindowExW(
            0,
            ListWindowClass_,
            ListWindowClass_,
            WS_OVERLAPPEDWINDOW ,
            CW_USEDEFAULT, CW_USEDEFAULT, 500, 700,
            nullptr ,
            nullptr,
            hInstance_,
            this
        );
        HMENU hMenu = CreateMenu();
        HMENU hFileMenu = CreatePopupMenu();

        AppendMenuW(
            hFileMenu,
            MF_STRING,
            ID_VIDEOPLAYBACK_LOADFOLDERTOPAH,
            L"LOADFOLDERTOPAH"
        );
        AppendMenuW(
            hFileMenu,
            MF_STRING,
            ID_VIDEOPLAYBACK_CLEAR_LIST,
            L"ClearList"
        );

        AppendMenuW(
            hMenu,
            MF_POPUP,
            reinterpret_cast<UINT_PTR>(hFileMenu),
            L"ファイル"
        );

        SetMenu(ListWindow_, hMenu);
        DrawMenuBar(ListWindow_);
        SetWindowTextW(ListWindow_, ListWindowClass_);
        ShowWindow(ListWindow_, SW_SHOW);
        UpdateWindow(ListWindow_);
        SetForegroundWindow(ListWindow_);

        return true;
    }
}
