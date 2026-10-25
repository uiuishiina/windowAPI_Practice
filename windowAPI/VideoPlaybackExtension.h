#pragma once

#include"ApplocationExtensionBase.h"
#include"Extension/MenuExtension.h"
#include"Extension/SubWindow.h"

#include<concepts>
#include<string>
#include<vector>
#include<array>
#include<memory>


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace VideoPlaybackExtension {

    enum class VideoFlags {
        mp3 = 1 << 0,
        wav = 1 << 1,
        mp4 = 1 << 2,
    };

    template<typename E>
    concept IsVideoFlags = std::same_as<E, VideoFlags>;

    template<IsVideoFlags... flags>
    unsigned int CreateVideoFlag(flags... flag) {
        return (0u | ... | static_cast<unsigned int>(flag));
    }

    /// <summary>
    /// 動画再生拡張機能
    /// </summary>
    class VideoPlaybackExtension final : public ApplicationExtensionBase, public SubWindow::SubWindow
    {
        WCHAR ErrorLogW[29] = L"VideoPlaybackExtension Error";
        CHAR ErrorLogA[29] = "VideoPlaybackExtension Error";
    public:
        template<IsVideoFlags... flags>
        explicit VideoPlaybackExtension(HWND hWnd, HINSTANCE hInstance,flags... flag) :
            ApplicationExtensionBase{ hWnd, hInstance },
            SubWindow{ L"VideoListWindow" },
            Flag_{ CreateVideoFlag(flag...) } {


            srand((unsigned int)time(NULL));


            const auto wc = CreateWindowClass();
            RegisterClassExW(&wc);

            AppMenu_ = std::make_unique<MenuExtension::MenuExtension>(hWnd_, hInstance_);

            auto file = MenuExtension::MenuFactory::CreatePopupMenuToInfo(
                {
                    MenuExtension::MenuFactory::MakeMenuItem(
                    CREATE_VIDEO_PLAYBACK_WINDOW,
                    L"ウィンドウ表示",
                    MFS_ENABLED
                    )
                }
            );
            
            bool result = MenuExtension::MenuFactory::AddSubMenu(
                AppMenu_->WindowMenu_,
                file,
                L"動画再生機能拡張",
                GetMenuItemCount(AppMenu_->WindowMenu_)
            );
            AppMenu_->AddMenuFunction(CREATE_VIDEO_PLAYBACK_WINDOW, [this](WPARAM wParam, LPARAM lParam) {

                if (SubhWnd_ == nullptr) {
                    CreateListWindow();
                    if (SubhWnd_) {
                        SetForegroundWindow(SubhWnd_); 
                        if (ListMenu_ && VideoPassContainer_.empty()) {
                            ListMenu_->CallMenuFunction(VIDEOPLAYBACK_LOADFOLDERTOPAH, wParam, lParam);
                        }
                        ShowConrainer();
                    }
                }
                else {
                    ShowWindow(SubhWnd_, SW_SHOW);
                    SetForegroundWindow(SubhWnd_);
                }
                });
            AppMenu_->SetMenuBar(hWnd_, AppMenu_->WindowMenu_);
        }

        /// <summary>
        /// ウィンドウクラス作成関数
        /// </summary>
        /// <returns>作成したウィンドウクラス</returns>
        const WNDCLASSEXW CreateWindowClass() const noexcept override {

            WNDCLASSEXW wc{};
            wc.cbSize = sizeof(wc);
            wc.lpfnWndProc = WndProc;
            wc.hInstance = hInstance_;
            wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            wc.hbrBackground = reinterpret_cast<HBRUSH>(
                COLOR_WINDOW + 1
                );
            wc.lpszClassName = ClassName_.c_str();

            return wc;
        }

        void ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
        
        /// <summary>
        /// サブウィンドウプロシージャ関数
        /// </summary>
        /// <param name="hWnd">ウィンドウハンドル</param>
        /// <param name="message">メッセージ</param>
        /// <param name="wParam">パラメーター</param>
        /// <param name="lParam">パラメーター</param>
        /// <returns>メッセージの返値</returns>
        LRESULT SubWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

        std::wstring SelectPath() const;

        void LoadFolderToPath(const std::wstring& path);

        void OpenFile(const std::wstring& path) const;

        void ClearContainer();

        void ShowConrainer();

        bool CreateListWindow() {

            if (!CreateSubWindow(hInstance_, 500, 700)) {
                return false;
            }

            ListMenu_ = std::make_unique<MenuExtension::MenuExtension>(SubhWnd_, hInstance_);

            auto load = MenuExtension::MenuFactory::CreatePopupMenuToInfo(
                {
                    MenuExtension::MenuFactory::MakeMenuItem(
                    VIDEOPLAYBACK_LOADFOLDERTOPAH,
                    L"ファイルLoad",
                    MFS_ENABLED
                    ),
                    MenuExtension::MenuFactory::MakeMenuItem(
                   VIDEOPLAYBACK_RANDOM_SECELT,
                    L"ランダム選択",
                    MFS_ENABLED),
                    MenuExtension::MenuFactory::MakeMenuItem(
                    VIDEOPLAYBACK_CLEAR_LIST,
                    L"Clear",
                    MFS_ENABLED
                    )
                }
            );

            bool result = MenuExtension::MenuFactory::AddSubMenu(
                ListMenu_->WindowMenu_,
                load,
                L"File",
                0
            );

            ListMenu_->AddMenuFunction(VIDEOPLAYBACK_LOADFOLDERTOPAH, [this](WPARAM wParam, LPARAM lParam) {

                if (auto path = SelectPath(); !path.empty()) {
                    LoadFolderToPath(path);
                    ShowConrainer();
                }
                });
            ListMenu_->AddMenuFunction(VIDEOPLAYBACK_RANDOM_SECELT, [this](WPARAM wParam, LPARAM lParam) {

                auto index = 0 + rand() % (VideoPassContainer_.size());

                SendMessage(ListBox_, LB_SETCURSEL, index, 0);

                if (index != LB_ERR) {
                    // 選択された index を利用する
                    const std::wstring& selected = VideoPassContainer_[index];
                    OpenFile(selected);
                }

                });
            ListMenu_->AddMenuFunction(VIDEOPLAYBACK_CLEAR_LIST, [this](WPARAM wParam, LPARAM lParam) {

                ClearContainer();
                });
            ListMenu_->AddMenuFunction(VIDEOPLAYBACK_DOUBLE_CLICK_LIST, [this](WPARAM wParam, LPARAM lParam) {

                if (HIWORD(wParam) == LBN_SELCHANGE) {
                    int index = static_cast<int>(
                        SendMessageW(
                            reinterpret_cast<HWND>(lParam),
                            LB_GETCURSEL,
                            0,
                            0
                        ));
                }
                else if (HIWORD(wParam) == LBN_DBLCLK) {
                    int index = static_cast<int>(
                        SendMessageW(
                            reinterpret_cast<HWND>(lParam),
                            LB_GETCURSEL,
                            0,
                            0
                        ));
                    if (index != LB_ERR) {
                        // 選択された index を利用する
                        const std::wstring& selected = VideoPassContainer_[index];
                        OpenFile(selected);
                    }
                }
                });
            ListMenu_->SetMenuBar(SubhWnd_, ListMenu_->WindowMenu_);

            return true;
        }

    private:

        unsigned int Flag_{};

        HWND ListBox_{};

        std::unique_ptr<MenuExtension::MenuExtension> AppMenu_{};

        std::unique_ptr<MenuExtension::MenuExtension> ListMenu_{};

        std::vector<std::wstring> VideoPassContainer_{};

        static const UINT CREATE_VIDEO_PLAYBACK_WINDOW = 32770;
        static const UINT VIDEOPLAYBACK_LOADFOLDERTOPAH = 32771;
        static const UINT VIDEOPLAYBACK_CLEAR_LIST = 32772;
        static const UINT VIDEOPLAYBACK_DOUBLE_CLICK_LIST = 32773;
        static const UINT VIDEOPLAYBACK_RANDOM_SECELT = 32774;

    };
}
