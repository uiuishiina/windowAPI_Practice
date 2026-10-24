
#include"VideoPlaybackExtension.h"

#include<filesystem>
#include<shellapi.h>
#include<shobjidl.h>
#include<wrl/client.h>

void VideoPlaybackExtension::VideoPlaybackExtension::ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case ID_VIDEOPLAYBACK_OPEN_WINDOW:
            if (ListWindow_ == nullptr) {
                CreateListWindow();
                if (ListWindow_) {
                    SetForegroundWindow(ListWindow_);
                }
            }
            else {
                ShowWindow(ListWindow_, SW_SHOW);
                SetForegroundWindow(ListWindow_);
            }
            break;
        }
        break;
    }
    }
}

LRESULT VideoPlaybackExtension::VideoPlaybackExtension::VideoListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
            reinterpret_cast<HMENU>(1001),
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
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case ID_VIDEOPLAYBACK_LOADFOLDERTOPAH:
            if (auto path = SelectPath(); !path.empty()) {
                LoadFolderToPath(path);
                ShowConrainer();
            }
            break;
        case ID_VIDEOPLAYBACK_CLEAR_LIST:
            ClearContainer();
            break;
        }
        if (
            wmId == 1001 &&
            HIWORD(wParam) == LBN_SELCHANGE
            ) {
            int index = static_cast<int>(
                SendMessageW(
                    reinterpret_cast<HWND>(lParam),
                    LB_GETCURSEL,
                    0,
                    0
                )
                );

            if (index != LB_ERR) {
                // 選択された index を利用する
                const std::wstring& selected = VideoPassContainer_[index];
                OpenFile(selected);
            }
        }
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;

    case WM_DESTROY:
        ListWindow_ = nullptr;
        ListBox_ = nullptr;
        return 0;
    }

    return DefWindowProcW(
        hWnd, message, wParam, lParam
    );
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


    ShowMessage(
        std::wstring{ L"path = [ " + path + L" ]\n" }.c_str(),
        L"VideoPlayback Load Folder To Path"
    );

    //  パス内走査
    try {
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(path)))
        {
            auto path_ = entry.path();
            if (entry.is_regular_file()) {

                //拡張子取得
                std::string ext = entry.path().extension().string();
                for (char& c : ext) {
                    c = std::tolower(c);
                }

                //判定
                if (Flag_ & static_cast<unsigned int>(videoFlags::mp3)) {
                    if (ext == ".mp3") {
                        VideoPassContainer_.push_back(path_.wstring());
                        continue;
                    }
                }
                if (Flag_ & static_cast<unsigned int>(videoFlags::wav)) {
                    if (ext == ".wav") {
                        VideoPassContainer_.push_back(path_.wstring());
                        continue;
                    }
                }
                if (Flag_ & static_cast<unsigned int>(videoFlags::mp4)) {
                    if (ext == ".mp4") {
                        VideoPassContainer_.push_back(path_.wstring());
                        continue;
                    }
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
    for (const auto& name : VideoPassContainer_) {
        SendMessageW(
            ListBox_,
            LB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>(name.c_str())
        );
    }
}