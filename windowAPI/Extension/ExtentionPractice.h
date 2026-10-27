#pragma once

#include"BitFlagExtension.h"
#include"SubWindow.h"

#include<memory>

namespace Practice {


    enum class PracticeFlags {
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

    enum PracticeFlagID : UINT {
        IDM_EXT_MP3 = 15001,
        IDM_EXT_WAV,
        IDM_EXT_MP4,
        IDM_EXT_EXE,
        IDM_EXT_PNG,
        IDM_EXT_JPG,

        IDM_EXT_NONE,
        IDM_EXT_ALL
    };

    struct PracticeMenuInfo : public BitFlagExtension::BitFlagMenuBase<PracticeFlags> {};

    /// <summary>
    /// ファイル読み込みフラグ設定構造体配列
    /// </summary>
    constexpr PracticeMenuInfo PracticeItems[] = {
    { IDM_EXT_MP3,  L"MP3",     PracticeFlags::mp3 },
    { IDM_EXT_WAV,  L"WAV",     PracticeFlags::wav },
    { IDM_EXT_MP4,  L"MP4",     PracticeFlags::mp4 },
    { IDM_EXT_EXE,  L"EXE",     PracticeFlags::exe },
    { IDM_EXT_PNG,  L"PNG",     PracticeFlags::png },
    { IDM_EXT_JPG,  L"JPG",     PracticeFlags::jpg },
    { IDM_EXT_NONE, L"NONE",    PracticeFlags::None },
    { IDM_EXT_ALL,  L"ALL",     PracticeFlags::All },
    };

    class PracticeExtention : public MenuExt::MenuExtension, public BitFlagExtension::BitFlagMenuExtension<PracticeFlags>
    {

    public:
        const wchar_t* name = L"Name";

        explicit PracticeExtention(HWND hWnd, HINSTANCE hInstance) :
            MenuExtension{ hWnd ,hInstance},
            BitFlagMenuExtension<PracticeFlags>{ PracticeFlags::mp3 }
        {

            IlitializeFlagMenu(
                WindowMenu_,
                name,
                std::span<const PracticeMenuInfo>{PracticeItems},
                this
            );

            SetNoneRange(PracticeFlags::None);
            SetAllRange(PracticeFlags::None);

            SetMenuBar(hWnd, WindowMenu_);
        }



    };
}