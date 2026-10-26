#include "SelectFileExtension.h"

#include<filesystem>
#include<shellapi.h>
#include<shobjidl.h>
#include<wrl/client.h>

#include<functional>
#include<format>

/// <summary>
/// 拡張機能用ウィンドウプロシージャ
/// </summary>
/// <param name="hWnd">親ウィンドウハンドル</param>
/// <param name="message">メッセージ</param>
/// <param name="wParam">パラメーター</param>
/// <param name="lParam">パラメーター</param>
void SelectFileExtension::SelectFileExtension::ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (AppMenu_ != nullptr) {
        AppMenu_->ExtensionProc(hWnd, message, wParam, lParam);
    }
}

/// <summary>
/// サブウィンドウプロシージャ関数
/// </summary>
/// <param name="hWnd">ウィンドウハンドル</param>
/// <param name="message">メッセージ</param>
/// <param name="wParam">パラメーター</param>
/// <param name="lParam">パラメーター</param>
/// <returns>メッセージの返値</returns>
LRESULT SelectFileExtension::SelectFileExtension::SubWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE: {
        ListBox_ = CreateWindowExW(
            WS_EX_CLIENTEDGE,
            L"LISTBOX",
            nullptr,
            WS_CHILD | WS_VISIBLE |
            WS_VSCROLL | LBS_NOTIFY |
            LBS_NOINTEGRALHEIGHT,
            0, 0, 400, 500,
            hWnd,
            reinterpret_cast<HMENU>(SELECT_FILE_DOUBLE_CLICK_LIST),
            GetModuleHandleW(nullptr),
            nullptr
        );
        return ListBox_ ? 0 : -1;
    }
    case WM_CTLCOLORLISTBOX:
    {
        HDC hdc = (HDC)wParam;

        // 文字色
        SetTextColor(hdc, RGB(0, 0, 0));

        // 背景色
        SetBkColor(hdc, RGB(230, 230, 230));

        // 背景ブラシを返す
        static HBRUSH hBrush = CreateSolidBrush(RGB(230, 230, 230));

        return (LRESULT)hBrush;
    }
    case WM_SIZE: {
        if (ListBox_) {
            MoveWindow(
                ListBox_,
                0, 0,
                LOWORD(lParam),
                HIWORD(lParam),
                TRUE
            );
        }
        return 0;
    }
    case WM_COMMAND:
    {
        if (ListMenu_ != nullptr) {
            ListMenu_->ExtensionProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;

    case WM_DESTROY:
        OnDestroyListWindow();
        return 0;
    }

    return DefWindowProcW(
        hWnd, message, wParam, lParam
    );
}

/// <summary>
/// フォルダーパス読み込み関数
/// </summary>
/// <returns>読み込んだパス</returns>
std::wstring SelectFileExtension::SelectFileExtension::SelectPath() const {

    Microsoft::WRL::ComPtr<IFileOpenDialog> dialog;

    HRESULT hr = CoCreateInstance(
        CLSID_FileOpenDialog,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&dialog)
    );

    if (FAILED(hr))
        return {};

    DWORD options = 0;
    dialog->GetOptions(&options);
    dialog->SetOptions(options | FOS_PICKFOLDERS);

    hr = dialog->Show(hWnd_);
    if (FAILED(hr)) {
        return {};
    }

    Microsoft::WRL::ComPtr<IShellItem> item;

    hr = dialog->GetResult(&item);
    if (FAILED(hr))
        return {};

    PWSTR selectedPath = nullptr;

    hr = item->GetDisplayName(SIGDN_FILESYSPATH, &selectedPath);
    if (FAILED(hr)) {
        return{};
    }

    std::wstring path{ selectedPath };

    CoTaskMemFree(selectedPath);
    return path;
}

/// <summary>
/// フォルダー内読み込み関数
/// </summary>
/// <param name="path">フォルダーパス</param>
void SelectFileExtension::SelectFileExtension::LoadFolderToPath(std::wstring path) {

    //  エラーチェック
    if (LoadFlag_.none()) {
        ShowMessage(L"FileExtension Flag Not Set", ErrorLogW);
        return;
    }
    if (!std::filesystem::exists(path)) {
        ShowMessage(L"Folder Path Not Found", ErrorLogW);
        return;
    }
    if (!std::filesystem::is_directory(path)) {
        ShowMessage(L"Path Not Directory", ErrorLogW);
        return;
    }

    ClearContainer();

    LoadedPath_ = path;
    
    //  パス内走査
    try {
        auto Count = DirectoryCount_;
        std::function<bool(const std::filesystem::path&)> Judge = [this](const std::filesystem::path& path) {

            //拡張子取得
            std::string ext = path.extension().string();
            for (char& c : ext) {
                c = std::tolower(c);
            }
            if (LoadFlag_.test(FileExtensionFlags::none)) {
                return false;
            }
            else if (LoadFlag_.test(FileExtensionFlags::all)) {
                return true;
            }

            //判定
            if (LoadFlag_.test(FileExtensionFlags::mp3)) {
                if (ext == ".mp3") {
                    return true;
                }
            }
            if (LoadFlag_.test(FileExtensionFlags::wav)) {
                if (ext == ".wav") {
                    return true;
                }
            }
            if (LoadFlag_.test(FileExtensionFlags::mp4)) {
                if (ext == ".mp4") {
                    return true;
                }
            }
            if (LoadFlag_.test(FileExtensionFlags::exe)) {
                if (ext == ".exe") {
                    return true;
                }
            }
            if (LoadFlag_.test(FileExtensionFlags::png)) {
                if (ext == ".png") {
                    return true;
                }
            }
            if (LoadFlag_.test(FileExtensionFlags::jpg)) {
                if (ext == ".jpg") {
                    return true;
                }
            }
            return false;
        };

        std::function<void(const std::filesystem::path& path)> func = [&, this](const std::filesystem::path& path) {
            for (const auto& item : std::filesystem::directory_iterator(path))
            {
                if (FileCountRange_ <= FilePassContainer_.size()) {
                    return;
                }
                auto path_ = item.path();
                if (item.is_regular_file()) {
                    if (Judge(path_)) {
                        FilePassContainer_.push_back(path_.wstring());
                    }
                }
                else if (item.is_directory() && Count > 0) {
                    --Count;
                    func(path_);
                    ++Count;
                }
            }
        };
        func(std::filesystem::path(path));
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        ShowMessage(static_cast<LPCSTR>(error.what()), ErrorLogA);
    }

    SetHeaderMenu();
}

/// <summary>
/// ファイル位置エクスプローラー表示関数
/// </summary>
/// <param name="path">表示するファイルパス</param>
void SelectFileExtension::SelectFileExtension::OpenFile(const std::wstring& path) const {

    std::wstring command{ L"/select,\"" + path + L"\"" };

    ShellExecuteW(
        NULL,
        L"open",
        L"explorer.exe",
        command.c_str(),
        NULL,
        SW_SHOWNORMAL
    );
}


/// <summary>
/// ディレクトリ位置エクスプローラー表示関数
/// </summary>
/// <param name="path">表示するディレクトリパス</param>
void SelectFileExtension::SelectFileExtension::OpenDirectory(const std::wstring& path) const {

    std::wstring command{ L"/open,\"" + path + L"\"" };

    ShellExecuteW(
        NULL,
        L"open",
        L"explorer.exe",
        command.c_str(),
        NULL,
        SW_SHOWNORMAL
    );
}

/// <summary>
/// ヘッダーメニュー設定関数
/// </summary>
void SelectFileExtension::SelectFileExtension::SetHeaderMenu() const {

    std::wstring text = LoadedPath_.empty() ? L"未設定" : LoadedPath_;
    std::wstring passText = L"読み込み開始位置パス : " + text;
    auto result = MenuExtension::MenuFactory::SetMenuHeaderToInfo(
        GetSubMenu(ListMenu_->WindowMenu_, 0),
        passText.c_str(),
        4
    );
    auto sizeText = L"ファイル数 : " + std::to_wstring(FilePassContainer_.size());
    result = MenuExtension::MenuFactory::SetMenuHeaderToInfo(
        GetSubMenu(ListMenu_->WindowMenu_, 0),
        sizeText.c_str(),
        5
    );
}

/// <summary>
/// フォルダー内再読み込み関数
/// </summary>
void SelectFileExtension::SelectFileExtension::ReloadPath() {

    LoadFolderToPath(LoadedPath_);
    ShowConrainer();
}

/// <summary>
/// パス配列初期化関数
/// </summary>
void SelectFileExtension::SelectFileExtension::ClearContainer() {

    FilePassContainer_.clear();
    LoadedPath_ = {};
    SendMessageW(
        ListBox_,
        LB_RESETCONTENT,
        0,
        0
    );
    SetHeaderMenu();
}

/// <summary>
/// パス配列内リスト表示関数
/// </summary>
void SelectFileExtension::SelectFileExtension::ShowConrainer() {

    SendMessageW(
        ListBox_,
        LB_RESETCONTENT,
        0,
        0
    );
    std::function<bool(const std::filesystem::path&)> Judge = [this](const std::filesystem::path& path) {

        //拡張子取得
        std::string ext = path.extension().string();
        for (char& c : ext) {
            c = std::tolower(c);
        }
        if (ShowFlag_.test(FileExtensionFlags::none)) {
            return false;
        }
        else if (ShowFlag_.test(FileExtensionFlags::all)) {
            return true;
        }

        //判定
        if (ShowFlag_.test(FileExtensionFlags::mp3)) {
            if (ext == ".mp3") {
                return true;
            }
        }
        if (ShowFlag_.test(FileExtensionFlags::wav)) {
            if (ext == ".wav") {
                return true;
            }
        }
        if (ShowFlag_.test(FileExtensionFlags::mp4)) {
            if (ext == ".mp4") {
                return true;
            }
        }
        if (ShowFlag_.test(FileExtensionFlags::exe)) {
            if (ext == ".exe") {
                return true;
            }
        }
        if (ShowFlag_.test(FileExtensionFlags::png)) {
            if (ext == ".png") {
                return true;
            }
        }
        if (ShowFlag_.test(FileExtensionFlags::jpg)) {
            if (ext == ".jpg") {
                return true;
            }
        }
        return false;
        };

    for (const auto& name : FilePassContainer_) {

        if (Judge(std::filesystem::path(name))) {
            SendMessageW(
                ListBox_,
                LB_ADDSTRING,
                0,
                reinterpret_cast<LPARAM>(name.c_str())
            );
        }
    }
    SetForegroundWindow(SubhWnd_);
}


/// <summary>
/// サブウィンドウ破棄時処理関数
/// </summary>
void SelectFileExtension::SelectFileExtension::OnDestroyListWindow() {

    ListMenu_->ClearMenuFunction();

    SubhWnd_ = nullptr;
    ListBox_ = nullptr;
    ListMenu_ = nullptr;
}