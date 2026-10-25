#include "SelectFileExtension.h"


#include<filesystem>
#include<shellapi.h>
#include<shobjidl.h>
#include<wrl/client.h>

#include<functional>

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
        DestroyListWindow();
        return 0;
    }

    return DefWindowProcW(
        hWnd, message, wParam, lParam
    );
}

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

void SelectFileExtension::SelectFileExtension::LoadFolderToPath(const std::wstring& path) {

    ClearContainer();

    //  エラーチェック
    if (Flag_ == FileExtensionFlags::None) {
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

    //  パス内走査
    try {
        auto Count = DirectoryCount_;

        std::function<bool(FileExtensionFlags, const std::filesystem::path&)> Judge = [this](FileExtensionFlags flag, const std::filesystem::path& path) {

            //拡張子取得
            std::string ext = path.extension().string();
            for (char& c : ext) {
                c = std::tolower(c);
            }

            //判定
            if (HasFlag(flag, FileExtensionFlags::mp3)) {
                if (ext == ".mp3") {
                    return true;
                }
            }
            if (HasFlag(flag, FileExtensionFlags::wav)) {
                if (ext == ".wav") {
                    return true;
                }
            }
            if (HasFlag(flag, FileExtensionFlags::mp4)) {
                if (ext == ".mp4") {
                    return true;
                }
            }
            if (HasFlag(flag, FileExtensionFlags::exe)) {
                if (ext == ".exe") {
                    return true;
                }
            }
            if (HasFlag(flag, FileExtensionFlags::png)) {
                if (ext == ".png") {
                    return true;
                }
            }
            if (HasFlag(flag, FileExtensionFlags::jpg)) {
                if (ext == ".jpg") {
                    return true;
                }
            }
            return false;
        };

        std::function<void(const std::filesystem::path& path)> func = [&, this](const std::filesystem::path& path) {
            for (const auto& item : std::filesystem::directory_iterator(path))
            {
                auto path_ = item.path();
                if (item.is_regular_file()) {
                    if (Judge(Flag_, path_)) {
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
}

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

void SelectFileExtension::SelectFileExtension::ClearContainer() {

    FilePassContainer_.clear();
    SendMessageW(
        ListBox_,
        LB_RESETCONTENT,
        0,
        0
    );
}

void SelectFileExtension::SelectFileExtension::ShowConrainer() {

    SendMessageW(
        ListBox_,
        LB_RESETCONTENT,
        0,
        0
    );
    for (const auto& name : FilePassContainer_) {
        SendMessageW(
            ListBox_,
            LB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>(name.c_str())
        );
    }
    SetForegroundWindow(SubhWnd_);
}


void SelectFileExtension::SelectFileExtension::DestroyListWindow() {

    ListMenu_->ClearMenuFunction();

    SubhWnd_ = nullptr;
    ListBox_ = nullptr;
    ListMenu_ = nullptr;

}