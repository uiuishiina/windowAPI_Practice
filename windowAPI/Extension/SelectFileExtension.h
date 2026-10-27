#pragma once

#include"../ApplocationExtensionBase.h"
#include"../Extension/MenuExtension.h"
#include"../Extension/SubWindow.h"

#include"BitFlag.h"

#include"BitFlagExtension.h"

#include<concepts>
#include<string>
#include<vector>
#include<array>
#include<memory>

namespace SelectFileExtension {

    /// <summary>
    /// ファイル読み込みフラグ列挙体
    /// </summary>
    enum class FileExtensionFlags {
        mp3,
        wav,
        mp4,
        exe,
        png,
        jpg,

        None,
        All,
        Count
    };

    /// <summary>
    /// ファイル読み込みフラグID列挙体
    /// </summary>
    enum FileExtensionFlagID : UINT {
        IDM_EXT_MP3 = 5001,
        IDM_EXT_WAV,
        IDM_EXT_MP4,
        IDM_EXT_EXE,
        IDM_EXT_PNG,
        IDM_EXT_JPG,

        IDM_EXT_NONE,
        IDM_EXT_ALL
    };

    /// <summary>
    /// ファイル読み込みフラグ設定構造体
    /// </summary>
    struct FileExtensionMenuInfo : public BitFlagExtension::BitFlagMenuBase<FileExtensionFlags> {};

    /// <summary>
    /// ファイル読み込みフラグ設定構造体配列
    /// </summary>
    constexpr FileExtensionMenuInfo FileExtensionItems[] = {
    { IDM_EXT_MP3,  L"MP3",     FileExtensionFlags::mp3 },
    { IDM_EXT_WAV,  L"WAV",     FileExtensionFlags::wav },
    { IDM_EXT_MP4,  L"MP4",     FileExtensionFlags::mp4 },
    { IDM_EXT_EXE,  L"EXE",     FileExtensionFlags::exe },
    { IDM_EXT_PNG,  L"PNG",     FileExtensionFlags::png },
    { IDM_EXT_JPG,  L"JPG",     FileExtensionFlags::jpg },
    { IDM_EXT_NONE, L"NONE",    FileExtensionFlags::None },
    { IDM_EXT_ALL,  L"ALL",     FileExtensionFlags::All },
    };

    /// <summary>
    /// 読み込みファイルリスト表示フラグID列挙体
    /// </summary>
    enum FileShowFlagID : UINT {
        IDM_EXT_SHOW_MP3 = IDM_EXT_ALL + 1,
        IDM_EXT_SHOW_WAV,
        IDM_EXT_SHOW_MP4,
        IDM_EXT_SHOW_EXE,
        IDM_EXT_SHOW_PNG,
        IDM_EXT_SHOW_JPG,

        IDM_EXT_SHOW_NONE,
        IDM_EXT_SHOW_ALL
    };

    /// <summary>
    /// 読み込みファイルリスト表示フラグ設定構造体
    /// </summary>
    struct FileShowMenuInfo : public BitFlagExtension::BitFlagMenuBase<FileExtensionFlags> {};

    constexpr FileShowMenuInfo FileShowMenuItems[] = {
        { IDM_EXT_SHOW_MP3,     L"MP3",     FileExtensionFlags::mp3 },
        { IDM_EXT_SHOW_WAV,     L"WAV",     FileExtensionFlags::wav },
        { IDM_EXT_SHOW_MP4,     L"MP4",     FileExtensionFlags::mp4 },
        { IDM_EXT_SHOW_EXE,     L"EXE",     FileExtensionFlags::exe },
        { IDM_EXT_SHOW_PNG,     L"PNG",     FileExtensionFlags::png },
        { IDM_EXT_SHOW_JPG,     L"JPG",     FileExtensionFlags::jpg },
        { IDM_EXT_SHOW_NONE,    L"NONE",    FileExtensionFlags::None },
        { IDM_EXT_SHOW_ALL,     L"ALL",     FileExtensionFlags::All },
    };

    /// <summary>
    /// ファイル読み込み拡張機能ID列挙体
    /// </summary>
    enum FileExtensionID : UINT {
        CREATE_SELECT_FILE_WINDOW   = 32780,
        SELECT_FILE_LOADFOLDER_TO_PATH,
        SELECT_FILE_LOADFOLDER_TO_LOADED_PATH,
        SELECT_FILE_CLEAR_LIST,
        SELECT_FILE_DOUBLE_CLICK_LIST,
        SELECT_FILE_RANDOM_SECELT
    };

    /// <summary>
    /// 読み込み階層設定ID列挙体
    /// </summary>
    enum DirectoryCountID : UINT {
        IDM_EXT_FIRST = IDM_EXT_SHOW_ALL + 1,
        IDM_EXT_SECOND,
        IDM_EXT_THERD,
        IDM_EXT_FORTH,
        IDM_EXT_FIFTH
    };

    /// <summary>
    /// 読み込み階層設定構造体
    /// </summary>
    struct FileDirectoryIDInfo {

        /// <summary>
        /// 対応する読み込み階層設定ID列挙体
        /// </summary>
        UINT    id;

        /// <summary>
        /// メニュー表示名
        /// </summary>
        LPCWSTR text;

        /// <summary>
        /// 対応する値設定
        /// </summary>
        UINT value;
    };

    /// <summary>
    /// 読み込み階層設定構造体配列
    /// </summary>
    constexpr FileDirectoryIDInfo FileDirectoryIDItems[] = {
        { IDM_EXT_FIRST     ,L"1"   ,1 },
        { IDM_EXT_SECOND    ,L"2"   ,2 },
        { IDM_EXT_THERD     ,L"3"   ,3 },
        { IDM_EXT_FORTH     ,L"4"   ,4 },
        { IDM_EXT_FIFTH     ,L"5"   ,5 },
    };

    /// <summary>
    /// ファイル読み込み制限設定ID列挙体
    /// </summary>
    enum FileCountRangeID : UINT {
        IDM_EXT_RENGE_10 = IDM_EXT_FIFTH + 1,
        IDM_EXT_RENGE_50,
        IDM_EXT_RENGE_100,
        IDM_EXT_RENGE_500,
        IDM_EXT_RENGE_1000,
        IDM_EXT_RENGE_NO_LIMIT
    };

    /// <summary>
    /// ファイル読み込み制限設定構造体
    /// </summary>
    struct FileCountRangeIDInfo {

        /// <summary>
        /// ファイル読み込み制限設定ID列挙体
        /// </summary>
        UINT    id;

        /// <summary>
        /// メニュー表示名
        /// </summary>
        LPCWSTR text;

        /// <summary>
        /// 対応する値
        /// </summary>
        UINT value;
    };


    /// <summary>
    /// ファイル読み込み制限設定構造体配列
    /// </summary>
    constexpr FileCountRangeIDInfo FileCountRangeIDItems[] = {
        { IDM_EXT_RENGE_10          , L"10"         ,10 },
        { IDM_EXT_RENGE_50          , L"50"         ,50 },
        { IDM_EXT_RENGE_100         , L"100"        ,100 },
        { IDM_EXT_RENGE_500         , L"500"        ,500 },
        { IDM_EXT_RENGE_1000        , L"1000"       ,1000 },
        { IDM_EXT_RENGE_NO_LIMIT    , L"NO_LIMIT"   ,INFINITE },
    };

    /// <summary>
    /// ファイル読み込みフラグ列挙体指定コンセプト
    /// </summary>
    template<typename E>
    concept IsFileExtensionFlags = std::same_as<E, FileExtensionFlags>;

    /// <summary>
    /// ファイル読み込み拡張機能
    /// </summary>
    class SelectFileExtension final : public ApplicationExtensionBase, public SubWindow::SubWindow
    {
    public:

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <typeparam name="...flags">開始フラグ設定</typeparam>
        /// <param name="hWnd">親ウィンドウハンドル</param>
        /// <param name="hInstance">ウィンドウインスタンス</param>
        /// <param name="...value">開始フラグ設定</param>
        template<IsFileExtensionFlags... flags>
        explicit SelectFileExtension(HWND hWnd, HINSTANCE hInstance, flags... flag) :
            ApplicationExtensionBase{ hWnd, hInstance },
            SubWindow{ L"SelectFileWindow" },
            LoadFlag_{ 
                std::make_unique<BitFlagExtension::BitFlagMenuExtension<FileExtensionFlags>>(flag...)
            },
            ShowFlag_{ 
                std::make_unique<BitFlagExtension::BitFlagMenuExtension<FileExtensionFlags>>(flag...)
            } {

            //  ウィンドウクラス登録
            const auto wc = CreateWindowClass();
            RegisterClassExW(&wc);

            //  親ウィンドウメニュー取得
            AppMenu_ = std::make_unique<MenuExt::MenuExtension>(hWnd_, hInstance_);

            //  クラスメニュー作成
            auto ExtensionMenu = MenuExt::MenuFactory::CreatePopupMenuToInfo(
                {
                    MenuExt::MenuFactory::MakeMenuItem(
                    CREATE_SELECT_FILE_WINDOW,
                    L"拡張機能ウィンドウ作成"
                    )
                }
            );

            //  読み込み階層設定メニュー情報作成
            std::vector<MENUITEMINFOW> Directoryitems{};
            for (auto& item : FileDirectoryIDItems) {

                auto index = item.value;

                Directoryitems.push_back(
                    MenuExt::MenuFactory::MakeMenuItem(
                        item.id,
                        item.text,
                        DirectoryCount_ == static_cast<unsigned int>(index) ? MFS_CHECKED : MFS_UNCHECKED
                    ));
            }

            //  ファイル読み込み制限設定メニュー情報作成
            std::vector<MENUITEMINFOW> Rangeitems{};
            for (auto& item : FileCountRangeIDItems) {

                Rangeitems.push_back(
                    MenuExt::MenuFactory::MakeMenuItem(
                        item.id,
                        item.text,
                        CheckFlag(FileCountRange_, item.value)
                    ));
            }

            
            
            //  オプションメニュー作成
            auto Option = MenuExt::MenuFactory::CreatePopupMenuToInfo({});
            
            //  ファイル読み込みフラグ設定メニュー情報作成
            LoadFlag_->IlitializeFlagMenu(
                Option,
                L"読み込みフラグ設定",
                std::span<const FileExtensionMenuInfo>{FileExtensionItems},
                AppMenu_.get(),
                [this](WPARAM wParam, LPARAM lParam) {
                    CallMenusFunc(wParam, lParam);
                }
            );

            //  メニュー情報追加
            MenuExt::MenuFactory::AddSubMenu(
                Option,
                L"読み込み階層",
                Directoryitems
            );
            MenuExt::MenuFactory::AddSubMenu(
                Option,
                L"読み込みファイル制限",
                Rangeitems
            );

            //  ファイル表示フラグ設定メニュー情報作成
            ShowFlag_->IlitializeFlagMenu(
                Option,
                L"ファイル表示設定",
                std::span<const FileShowMenuInfo>{FileShowMenuItems},
                AppMenu_.get(),
                [this](WPARAM wParam, LPARAM lParam) {
                    CallMenusFunc(wParam, lParam);
                }
            );

            LoadFlag_->SetNoneRange(FileExtensionFlags::None);
            LoadFlag_->SetAllRange(FileExtensionFlags::None);

            ShowFlag_->SetNoneRange(FileExtensionFlags::None);
            ShowFlag_->SetAllRange(FileExtensionFlags::None);

            LoadFlag_->ReloadAllFlagMenu(AppMenu_->WindowMenu_);
            ShowFlag_->ReloadAllFlagMenu(AppMenu_->WindowMenu_);

            MenuExt::MenuFactory::AddMenuToInfo(
                ExtensionMenu,
                MenuExt::MenuFactory::MakeMenuSeparator()
            );
            MenuExt::MenuFactory::AddSubMenu(
                ExtensionMenu,
                Option,
                L"Option"
            );

            //  クラスメニュー追加
            MenuExt::MenuFactory::AddSubMenu(
                AppMenu_->WindowMenu_,
                ExtensionMenu,
                L"ファイル取得機能拡張",
                GetMenuItemCount(AppMenu_->WindowMenu_)
            );


            //  クラスメニューに関数追加
            AppMenu_->AddMenuFunction(CREATE_SELECT_FILE_WINDOW, [this](WPARAM wParam, LPARAM lParam) {

                if (SubhWnd_ == nullptr) {

                    //  拡張機能用ウィンドウ作成
                    CreateListWindow();
                    if (SubhWnd_) {

                        //  ウィンドウが作成できたなら
                        SetForegroundWindow(SubhWnd_);
                        if (ListMenu_ && FilePassContainer_.empty()) {
                            ListMenu_->CallMenuFunction(SELECT_FILE_LOADFOLDER_TO_PATH, wParam, lParam);
                        }
                        ShowConrainer();
                    }
                }
                else {

                    //  拡張機能用ウィンドウ表示
                    ShowWindow(SubhWnd_, SW_SHOW);
                    SetForegroundWindow(SubhWnd_);
                }
                });


            //  読み込み階層変更
            for (auto& item : FileDirectoryIDItems) {
                auto toggleFunc = [&, id = item.id]
                (WPARAM wParam, LPARAM lParam) {

                    DirectoryCount_ = item.value;

                    for (auto& value : FileDirectoryIDItems) {

                        auto check = CheckFlag(DirectoryCount_, value.value);

                        MenuExt::MenuFactory::SetMenuToInfo(
                            AppMenu_->WindowMenu_,
                            MIIM_STATE,
                            check,
                            value.id
                        );
                        if (SubhWnd_) {
                            MenuExt::MenuFactory::SetMenuToInfo(
                                ListMenu_->WindowMenu_,
                                MIIM_STATE,
                                check,
                                value.id
                            );
                        }
                    }

                    };
                AppMenu_->AddMenuFunction(item.id, toggleFunc);
            }

            //  読み込みファイル制限変更
            for (auto& item : FileCountRangeIDItems) {
                auto toggleFunc = [&, id = item.id]
                (WPARAM wParam, LPARAM lParam) {

                    FileCountRange_ = item.value;

                    for (auto& value : FileCountRangeIDItems) {

                        auto check = CheckFlag(FileCountRange_, value.value);

                        MenuExt::MenuFactory::SetMenuToInfo(
                            AppMenu_->WindowMenu_,
                            MIIM_STATE,
                            check,
                            value.id
                        );
                        if (SubhWnd_) {
                            MenuExt::MenuFactory::SetMenuToInfo(
                                ListMenu_->WindowMenu_,
                                MIIM_STATE,
                                check,
                                value.id
                            );
                        }
                    }

                    };
                AppMenu_->AddMenuFunction(item.id, toggleFunc);
            }

            //  メニューバー表示
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
            wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
            wc.lpszClassName = ClassName_.c_str();

            return wc;
        }

        /// <summary>
        /// 拡張機能用ウィンドウプロシージャ
        /// </summary>
        /// <param name="hWnd">親ウィンドウハンドル</param>
        /// <param name="message">メッセージ</param>
        /// <param name="wParam">パラメーター</param>
        /// <param name="lParam">パラメーター</param>
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

        /// <summary>
        /// サブウィンドウ作成関数
        /// </summary>
        /// <returns></returns>
        bool CreateListWindow() {

            //サブウィンドウ作成関数
            if (!CreateSubWindow(hInstance_, 500, 700)) {
                return false;
            }

            //  サブウィンドウメニュー取得
            ListMenu_ = std::make_unique<MenuExt::MenuExtension>(SubhWnd_, hInstance_);

            //  ファイルメニュー作成
            auto file = MenuExt::MenuFactory::CreatePopupMenuToInfo(
                {

                    MenuExt::MenuFactory::MakeMenuItem(
                    SELECT_FILE_LOADFOLDER_TO_PATH,
                    L"ファイルLoad"
                    ),
                    MenuExt::MenuFactory::MakeMenuItem(
                    SELECT_FILE_RANDOM_SECELT,
                    L"ランダム選択"
                    ),
                    MenuExt::MenuFactory::MakeMenuItem(
                    SELECT_FILE_CLEAR_LIST,
                    L"Clear"
                    ),
                    MenuExt::MenuFactory::MakeMenuSeparator(),
                    MenuExt::MenuFactory::MakeMenuItem(
                    SELECT_FILE_LOADFOLDER_TO_LOADED_PATH,
                    L"読み込み開始位置パス : 未設定"
                    ),
                    MenuExt::MenuFactory::MakeMenuHeader(
                    L"ファイル数 : 0"
                    )
                }
            );
            
            //  読み込み階層設定メニュー情報作成
            std::vector<MENUITEMINFOW> Directoryitems{};
            for (auto& item : FileDirectoryIDItems) {

                auto index = item.value;

                Directoryitems.push_back(
                    MenuExt::MenuFactory::MakeMenuItem(
                        item.id,
                        item.text,
                        CheckFlag(DirectoryCount_,index)
                    ));
            }

            //  ファイル読み込み制限設定メニュー情報作成
            std::vector<MENUITEMINFOW> Rangeitems{};
            for (auto& item : FileCountRangeIDItems) {

                Rangeitems.push_back(
                    MenuExt::MenuFactory::MakeMenuItem(
                        item.id,
                        item.text,
                        CheckFlag(FileCountRange_, item.value)
                    ));
            }


            //  オプションメニュー作成
            auto option = MenuExt::MenuFactory::CreatePopupMenuToInfo({});

            //  ファイル読み込みフラグ設定メニュー情報作成
            LoadFlag_->IlitializeFlagMenu(
                option,
                L"読み込みフラグ設定",
                std::span<const FileExtensionMenuInfo>{FileExtensionItems},
                ListMenu_.get(),
                [this](WPARAM wParam, LPARAM lParam) {
                    ShowConrainer();
                    CallMenusFunc(wParam, lParam);

                    if (!LoadedPath_.empty()) {
                        ReloadPath();
                    }
                }
            );
            
            //  メニュー情報追加
            MenuExt::MenuFactory::AddSubMenu(
                option,
                L"読み込み階層",
                Directoryitems
            );
            MenuExt::MenuFactory::AddSubMenu(
                option,
                L"読み込みファイル制限",
                Rangeitems
            );

            //  ファイル表示フラグ設定メニュー情報作成
            ShowFlag_->IlitializeFlagMenu(
                option,
                L"ファイル表示設定",
                std::span<const FileShowMenuInfo>{FileShowMenuItems},
                ListMenu_.get(),
                [this](WPARAM wParam, LPARAM lParam) {
                    ShowConrainer();
                    CallMenusFunc(wParam, lParam);
                }
            );

            //  サブウィンドウメニュー追加
            MenuExt::MenuFactory::AddSubMenu(
                ListMenu_->WindowMenu_,
                file,
                L"File"
            );

            //  サブウィンドウメニュー追加
            MenuExt::MenuFactory::AddSubMenu(
                ListMenu_->WindowMenu_,
                option,
                L"オプション"
            );

            //  サブウィンドウメニューに関数追加
            ListMenu_->AddMenuFunction(SELECT_FILE_LOADFOLDER_TO_PATH, [this](WPARAM wParam, LPARAM lParam) {

                //  ファイル読み込み
                if (auto path = SelectPath(); !path.empty()) {
                    LoadFolderToPath(path);
                    ShowConrainer();
                }
                });

            ListMenu_->AddMenuFunction(SELECT_FILE_LOADFOLDER_TO_LOADED_PATH, [this](WPARAM wParam, LPARAM lParam) {

                if (!LoadedPath_.empty()) {
                    OpenDirectory(LoadedPath_);
                }
                });

            ListMenu_->AddMenuFunction(SELECT_FILE_CLEAR_LIST, [this](WPARAM wParam, LPARAM lParam) {

                //  読み込み配列初期化
                ClearContainer();
                });

            ListMenu_->AddMenuFunction(SELECT_FILE_RANDOM_SECELT, [this](WPARAM wParam, LPARAM lParam) {

                //  ランダムセレクト
                auto index = 0 + rand() % (FilePassContainer_.size());

                SendMessage(ListBox_, LB_SETCURSEL, index, 0);

                if (index != LB_ERR) {
                    // 選択された index を利用する
                    const std::wstring& selected = FilePassContainer_[index];
                    OpenFile(selected);
                }

                });

            ListMenu_->AddMenuFunction(SELECT_FILE_DOUBLE_CLICK_LIST, [this](WPARAM wParam, LPARAM lParam) {

                //  ファイルリスト選択
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
            

            //  読み込み階層変更
            for (auto& item : FileDirectoryIDItems) {
                auto toggleFunc = [&, id = item.id]
                (WPARAM wParam, LPARAM lParam) {

                    DirectoryCount_ = item.value;

                    for (auto& value : FileDirectoryIDItems) {

                        auto check = DirectoryCount_ == value.value ? MFS_CHECKED : MFS_UNCHECKED;

                        MenuExt::MenuFactory::SetMenuToInfo(
                            AppMenu_->WindowMenu_,
                            MIIM_STATE,
                            check,
                            value.id
                        );
                        MenuExt::MenuFactory::SetMenuToInfo(
                            ListMenu_->WindowMenu_,
                            MIIM_STATE,
                            check,
                            value.id
                        );
                    }

                    if (!LoadedPath_.empty()) {
                        ReloadPath();
                    }

                    };
                ListMenu_->AddMenuFunction(item.id, toggleFunc);
            }


            //  読み込みファイル制限変更
            for (auto& item : FileCountRangeIDItems) {
                auto toggleFunc = [&, id = item.id]
                (WPARAM wParam, LPARAM lParam) {

                    FileCountRange_ = item.value;

                    for (auto& value : FileCountRangeIDItems) {

                        auto check = CheckFlag(FileCountRange_, value.value);

                        MenuExt::MenuFactory::SetMenuToInfo(
                            AppMenu_->WindowMenu_,
                            MIIM_STATE,
                            check,
                            value.id
                        );
                        MenuExt::MenuFactory::SetMenuToInfo(
                            ListMenu_->WindowMenu_,
                            MIIM_STATE,
                            check,
                            value.id
                        );
                    }

                    if (!LoadedPath_.empty()) {
                        ReloadPath();
                    }

                    };
                ListMenu_->AddMenuFunction(item.id, toggleFunc);
            }

            //  サブウィンドウメニューバー設定
            ListMenu_->SetMenuBar(SubhWnd_, ListMenu_->WindowMenu_);

            return true;
        }

        /// <summary>
        /// フォルダーパス読み込み関数
        /// </summary>
        /// <returns>読み込んだパス</returns>
        std::wstring SelectPath() const;

        /// <summary>
        /// フォルダー内読み込み関数
        /// </summary>
        /// <param name="path">フォルダーパス</param>
        void LoadFolderToPath(std::wstring path);

        /// <summary>
        /// ファイル位置エクスプローラー表示関数
        /// </summary>
        /// <param name="path">表示するファイルパス</param>
        void OpenFile(const std::wstring& path) const;

        /// <summary>
        /// ディレクトリ位置エクスプローラー表示関数
        /// </summary>
        /// <param name="path">表示するディレクトリパス</param>
        void OpenDirectory(const std::wstring& path) const;

        /// <summary>
        /// ヘッダーメニュー設定関数
        /// </summary>
        void SetHeaderMenu() const;

        /// <summary>
        /// フォルダー内再読み込み関数
        /// </summary>
        void ReloadPath();

        /// <summary>
        /// パス配列初期化関数
        /// </summary>
        void ClearContainer();

        /// <summary>
        /// パス配列内リスト表示関数
        /// </summary>
        void ShowConrainer();

        /// <summary>
        /// サブウィンドウ破棄時処理関数
        /// </summary>
        void OnDestroyListWindow();

        /// <summary>
        /// フラグチェック関数
        /// </summary>
        /// <param name="value">ファイル読み込みフラグ</param>
        /// <returns>チェック識別子</returns>
        UINT CheckFlag(HandyItem::bit::BitFlag<FileExtensionFlags>& flag ,FileExtensionFlags value) {
            return flag.test(value) ? MFS_CHECKED : MFS_UNCHECKED;
        }

        /// <summary>
        /// フラグチェック関数
        /// </summary>
        /// <param name="value">チェックする値</param>
        /// <param name="value">識別値</param>
        /// <returns>チェック識別子</returns>
        UINT CheckFlag(UINT flag,UINT value) {
            return flag == value ? MFS_CHECKED : MFS_UNCHECKED;
        }

    private:

        /// <summary>
        /// ファイル読み込みフラグ
        /// </summary>
        std::unique_ptr<BitFlagExtension::BitFlagMenuExtension<FileExtensionFlags>> LoadFlag_{};

        std::unique_ptr<BitFlagExtension::BitFlagMenuExtension<FileExtensionFlags>> ShowFlag_{};

        /// <summary>
        /// 読み込み階層設定変数
        /// </summary>
        UINT DirectoryCount_ = 1;

        /// <summary>
        /// ファイル読み込み制限設定変数
        /// </summary>
        UINT FileCountRange_ = INFINITE;

        /// <summary>
        /// ファイルパス表示ボックスハンドル
        /// </summary>
        HWND ListBox_{};

        /// <summary>
        /// 親ウィンドウメニュー
        /// </summary>
        std::unique_ptr<MenuExt::MenuExtension> AppMenu_{};

        /// <summary>
        /// サブウィンドウメニュー
        /// </summary>
        std::unique_ptr<MenuExt::MenuExtension> ListMenu_{};

        /// <summary>
        /// ファイルパス配列
        /// </summary>
        std::vector<std::wstring> FilePassContainer_{};

        /* ===== エラーログ設定 ===== */

        WCHAR ErrorLogW[29] = L"SelectFileExtension Error";
        CHAR ErrorLogA[29] = "SelectFileExtension Error";

        std::wstring LoadedPath_{};


        void CallMenusFunc(WPARAM wParam,LPARAM lParam) {

            auto id = LOWORD(wParam);
            ShowFlag_->ReloadAllFlagMenu(AppMenu_->WindowMenu_);
            if (ListMenu_) {
                ShowFlag_->ReloadAllFlagMenu(ListMenu_->WindowMenu_);
            }
        }
    };
}