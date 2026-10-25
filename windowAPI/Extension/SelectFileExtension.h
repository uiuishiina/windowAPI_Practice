#pragma once


#include"../ApplocationExtensionBase.h"
#include"../Extension/MenuExtension.h"
#include"../Extension/SubWindow.h"


#include<concepts>
#include<string>
#include<vector>
#include<array>
#include<memory>

namespace SelectFileExtension {

    enum class FileExtensionFlags :uint32_t {
        None = 0,
        mp3 = 1 << 0,
        wav = 1 << 1,
        mp4 = 1 << 2,
        exe = 1 << 3,
        png = 1 << 4,
        jpg = 1 << 5
    };

    struct FileExtensionMenuInfo {
        UINT id;
        LPCWSTR text;
        FileExtensionFlags flag;
    };

    enum : UINT {
        IDM_EXT_MP3 = 5001,
        IDM_EXT_WAV,
        IDM_EXT_MP4,
        IDM_EXT_EXE,
        IDM_EXT_PNG,
        IDM_EXT_JPG
    };

    constexpr FileExtensionMenuInfo FileExtensionItems[] = {
    { IDM_EXT_MP3, L"MP3", FileExtensionFlags::mp3 },
    { IDM_EXT_WAV, L"WAV", FileExtensionFlags::wav },
    { IDM_EXT_MP4, L"MP4", FileExtensionFlags::mp4 },
    { IDM_EXT_EXE, L"EXE", FileExtensionFlags::exe },
    { IDM_EXT_PNG, L"PNG", FileExtensionFlags::png },
    { IDM_EXT_JPG, L"JPG", FileExtensionFlags::jpg }
    };

    constexpr FileExtensionFlags operator|(
        FileExtensionFlags lhs,
        FileExtensionFlags rhs
        ) {
        return static_cast<FileExtensionFlags>(
            static_cast<uint32_t>(lhs) |
            static_cast<uint32_t>(rhs)
            );
    }

    constexpr FileExtensionFlags operator&(
        FileExtensionFlags lhs,
        FileExtensionFlags rhs
        ) {
        return static_cast<FileExtensionFlags>(
            static_cast<uint32_t>(lhs) &
            static_cast<uint32_t>(rhs)
            );
    }

    constexpr FileExtensionFlags operator~(
        FileExtensionFlags value
        ) {
        return static_cast<FileExtensionFlags>(
            ~static_cast<uint32_t>(value)
            );
    }

    constexpr bool HasFlag(
        FileExtensionFlags value,
        FileExtensionFlags flag
    ) {
        return (value & flag) != FileExtensionFlags::None;
    }

    constexpr FileExtensionFlags ToggleExtension(
        FileExtensionFlags value,
        FileExtensionFlags flag
    ) {
        if (HasFlag(value, flag)) {
            return value & ~flag;
        }
        else {
            return value | flag;
        }
    }

    constexpr UINT GetCheckState(
        FileExtensionFlags flags,
        FileExtensionFlags flag
    ) {
        return HasFlag(flags, flag)
            ? MFS_CHECKED
            : MFS_UNCHECKED;
    }

    template<typename E>
    concept IsFileExtensionFlags = std::same_as<E, FileExtensionFlags>;

    template<IsFileExtensionFlags... flags>
    FileExtensionFlags CreateExtensionFlags(flags... flag) {
        return (FileExtensionFlags::None | ... | (flag));
    };

    class SelectFileExtension final : public ApplicationExtensionBase, public SubWindow::SubWindow
    {
    public:

        template<IsFileExtensionFlags... flags>
        explicit SelectFileExtension(HWND hWnd, HINSTANCE hInstance, flags... flag) :
            ApplicationExtensionBase{ hWnd, hInstance },
            SubWindow{ L"SelectFileWindow" },
            Flag_{ CreateExtensionFlags(flag...) } {

            const auto wc = CreateWindowClass();
            RegisterClassExW(&wc);

            AppMenu_ = std::make_unique<MenuExtension::MenuExtension>(hWnd_, hInstance_);

            auto file = MenuExtension::MenuFactory::CreatePopupMenuToInfo(
                {
                    MenuExtension::MenuFactory::MakeMenuItem(
                    CREATE_SELECT_FILE_WINDOW,
                    L"ウィンドウ表示",
                    MFS_ENABLED
                    )
                }
            );

            bool result = MenuExtension::MenuFactory::AddSubMenu(
                AppMenu_->WindowMenu_,
                file,
                L"ファイル取得機能拡張",
                GetMenuItemCount(AppMenu_->WindowMenu_)
            );
            AppMenu_->AddMenuFunction(CREATE_SELECT_FILE_WINDOW, [this](WPARAM wParam, LPARAM lParam) {

                if (SubhWnd_ == nullptr) {
                    CreateListWindow();
                    if (SubhWnd_) {
                        SetForegroundWindow(SubhWnd_);
                        if (ListMenu_ && FilePassContainer_.empty()) {
                            ListMenu_->CallMenuFunction(SELECT_FILE_LOADFOLDERTOPAH, wParam, lParam);
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


        bool CreateListWindow() {

            if (!CreateSubWindow(hInstance_, 500, 700)) {
                return false;
            }

            ListMenu_ = std::make_unique<MenuExtension::MenuExtension>(SubhWnd_, hInstance_);

            auto load = MenuExtension::MenuFactory::CreatePopupMenuToInfo(
                {
                    MenuExtension::MenuFactory::MakeMenuItem(
                    SELECT_FILE_LOADFOLDERTOPAH,
                    L"ファイルLoad",
                    MFS_ENABLED
                    ),
                    MenuExtension::MenuFactory::MakeMenuItem(
                    SELECT_FILE_RANDOM_SECELT,
                    L"ランダム選択",
                    MFS_ENABLED
                    ),
                    MenuExtension::MenuFactory::MakeMenuItem(
                    SELECT_FILE_CLEAR_LIST,
                    L"Clear",
                    MFS_ENABLED
                    )
                }
            );

            std::vector<MENUITEMINFOW> items{};
            for (auto& ex : FileExtensionItems) {
                items.push_back(
                    MenuExtension::MenuFactory::MakeMenuItem(
                    ex.id,
                    ex.text,
                    GetCheckState(Flag_, ex.flag)
                ));
            }

            auto option = MenuExtension::MenuFactory::CreatePopupMenuToInfo({});
            MenuExtension::MenuFactory::AddSubMenu(
                option,
                L"読み込みフラグ設定",
                items
            );

            MenuExtension::MenuFactory::AddSubMenu(
                ListMenu_->WindowMenu_,
                load,
                L"File"
            );
            MenuExtension::MenuFactory::AddSubMenu(
                ListMenu_->WindowMenu_,
                option,
                L"Option"
            );

            ListMenu_->AddMenuFunction(SELECT_FILE_LOADFOLDERTOPAH, [this](WPARAM wParam, LPARAM lParam) {

                if (auto path = SelectPath(); !path.empty()) {
                    LoadFolderToPath(path);
                    ShowConrainer();
                }
                });
            ListMenu_->AddMenuFunction(SELECT_FILE_RANDOM_SECELT, [this](WPARAM wParam, LPARAM lParam) {

                auto index = 0 + rand() % (FilePassContainer_.size());

                SendMessage(ListBox_, LB_SETCURSEL, index, 0);

                if (index != LB_ERR) {
                    // 選択された index を利用する
                    const std::wstring& selected = FilePassContainer_[index];
                    OpenFile(selected);
                }

                });
            ListMenu_->AddMenuFunction(SELECT_FILE_CLEAR_LIST, [this](WPARAM wParam, LPARAM lParam) {

                ClearContainer();
                });
            ListMenu_->AddMenuFunction(SELECT_FILE_DOUBLE_CLICK_LIST, [this](WPARAM wParam, LPARAM lParam) {

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
                        const std::wstring& selected = FilePassContainer_[index];
                        OpenFile(selected);
                    }
                }
                });

            for (auto& info : FileExtensionItems) {
                auto toggleFunc = [&, flag = info.flag, id = info.id]
                (WPARAM wParam, LPARAM lParam) {

                    Flag_ = ToggleExtension(Flag_, flag);

                    // メニューのチェック状態を更新する
                    const auto state = GetCheckState(
                        Flag_,
                        flag
                    );

                    MenuExtension::MenuFactory::SetMenuToInfo(
                        SubhWnd_,
                        ListMenu_->WindowMenu_,
                        MIIM_STATE,
                        state,
                        id
                    );
                };
                ListMenu_->AddMenuFunction(info.id, toggleFunc);
            }

            

            ListMenu_->SetMenuBar(SubhWnd_, ListMenu_->WindowMenu_);

            return true;
        }


        std::wstring SelectPath() const;

        void LoadFolderToPath(const std::wstring& path);

        void OpenFile(const std::wstring& path) const;

        void ClearContainer();

        void ShowConrainer();

        void DestroyListWindow();

    private:

        FileExtensionFlags Flag_{};

        unsigned int DirectoryCount_ = 5;

        HWND ListBox_{};

        std::unique_ptr<MenuExtension::MenuExtension> AppMenu_{};

        std::unique_ptr<MenuExtension::MenuExtension> ListMenu_{};

        std::vector<std::wstring> FilePassContainer_{};

        static const UINT CREATE_SELECT_FILE_WINDOW = 32780;
        static const UINT SELECT_FILE_LOADFOLDERTOPAH = 32781;
        static const UINT SELECT_FILE_CLEAR_LIST = 32782;
        static const UINT SELECT_FILE_DOUBLE_CLICK_LIST = 32783;
        static const UINT SELECT_FILE_RANDOM_SECELT = 32784;

        WCHAR ErrorLogW[29] = L"SelectFileExtension Error";
        CHAR ErrorLogA[29] = "SelectFileExtension Error";

    };

}