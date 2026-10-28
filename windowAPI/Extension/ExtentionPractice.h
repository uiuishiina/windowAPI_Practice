#pragma once

#include"ExclusiveMenuExtension.h"
#include"SubWindow.h"

#include<memory>

namespace Practice {

    enum PracticeFlagID : UINT {
        IDM_EXT_PF1 = 15001,
        IDM_EXT_PF2,
        IDM_EXT_PF3,
        IDM_EXT_PF4,
        IDM_EXT_PF5,
        IDM_EXT_PF6
    };

    struct PracticeMenuInfo : public ExclusiveMenuExtension::ExclusiveMenuBase<UINT> {};

    constexpr PracticeMenuInfo PracticeItems[] = {
    { IDM_EXT_PF1,  L"1",     1 },
    { IDM_EXT_PF2,  L"2",     2 },
    { IDM_EXT_PF3,  L"3",     3 },
    { IDM_EXT_PF4,  L"4",     4 },
    { IDM_EXT_PF5,  L"5",     5 },
    { IDM_EXT_PF6,  L"6",     6 }
    };

    class PracticeExtention : public MenuExt::MenuExtension, public ExclusiveMenuExtension::ExclusiveMenuExtension<UINT, PracticeMenuInfo>
    {

    public:
        const wchar_t* name = L"Name";

        explicit PracticeExtention(HWND hWnd, HINSTANCE hInstance) :
            MenuExtension{ hWnd ,hInstance},
            ExclusiveMenuExtension{ PracticeMenuInfo{ IDM_EXT_PF1,  L"1",     1 } }
        {

            InitializeExclusiveMenu(
                WindowMenu_,
                name,
                this
            );

            SetMenuBar(hWnd, WindowMenu_);
        }



    };
}