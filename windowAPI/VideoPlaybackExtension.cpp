
#include"VideoPlaybackExtension.h"

#include<filesystem>
#include<shellapi.h>
#include<shobjidl.h>
#include<wrl/client.h>

void VideoPlaybackExtension::VideoPlaybackExtension::ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (AppMenu_ != nullptr) {
        AppMenu_->ExtensionProc(hWnd, message, wParam, lParam);
    }
}

LRESULT VideoPlaybackExtension::VideoPlaybackExtension::SubWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
            reinterpret_cast<HMENU>(VIDEOPLAYBACK_DOUBLE_CLICK_LIST),
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
        SubhWnd_ = nullptr;
        ListBox_ = nullptr;
        ListMenu_ = nullptr;
        return 0;
    }

    return DefWindowProcW(
        hWnd, message, wParam, lParam
    );
}


inline bool JudgeExtension(unsigned int flag, const std::filesystem::path& path) {

    //拡張子取得
    std::string ext = path.extension().string();
    for (char& c : ext) {
        c = std::tolower(c);
    }

    //判定
    if (flag & static_cast<unsigned int>(VideoPlaybackExtension::VideoFlags::mp3)) {
        if (ext == ".mp3") {
            return true;
        }
    }
    if (flag & static_cast<unsigned int>(VideoPlaybackExtension::VideoFlags::wav)) {
        if (ext == ".wav") {
            return true;
        }
    }
    if (flag & static_cast<unsigned int>(VideoPlaybackExtension::VideoFlags::mp4)) {
        if (ext == ".mp4") {
            return true;
        }
    }
    return false;
}


std::wstring VideoPlaybackExtension::VideoPlaybackExtension::SelectPath() const {

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

void VideoPlaybackExtension::VideoPlaybackExtension::LoadFolderToPath(const std::wstring& path) {

    ClearContainer();

    //  エラーチェック
    if (Flag_ == 0) {
        ShowMessage(L"Video Flag Not Set", ErrorLogW);
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
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(path)))
        {
            auto path_ = entry.path();
            
            if (entry.is_regular_file()) {
                if (JudgeExtension(Flag_, path_)) {
                    VideoPassContainer_.push_back(path_.wstring());
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        ShowMessage(static_cast<LPCSTR>(error.what()), ErrorLogA);
    }
}

void VideoPlaybackExtension::VideoPlaybackExtension::OpenFile(const std::wstring& path) const {

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

void VideoPlaybackExtension::VideoPlaybackExtension::ClearContainer() {
    VideoPassContainer_.clear();
    SendMessageW(
        ListBox_,
        LB_RESETCONTENT,
        0,
        0
    );
}

void VideoPlaybackExtension::VideoPlaybackExtension::ShowConrainer() {

    SendMessageW(
        ListBox_,
        LB_RESETCONTENT,
        0,
        0
    );
    for (const auto& name : VideoPassContainer_) {
        SendMessageW(
            ListBox_,
            LB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>(name.c_str())
        );
    }
    SetForegroundWindow(SubhWnd_);
}