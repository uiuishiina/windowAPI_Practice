#pragma once

#include"../ApplocationExtensionBase.h"

#include<functional>
#include<unordered_map>
#include<span>

namespace MenuExtension {

    /// <summary>
    /// メニュー拡張機能
    /// </summary>
    class MenuExtension final : public ApplicationExtensionBase
    {
    public:

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="hWnd">拡張するウィンドウハンドル</param>
        /// <param name="hInstance">ウィンドウインスタンス</param>
        explicit MenuExtension(
            HWND hWnd, 
            HINSTANCE hInstance
        ) :
            ApplicationExtensionBase(hWnd, hInstance) {

            WindowMenu_ = GetMenu(hWnd_);
            if (WindowMenu_ == nullptr) {
                WindowMenu_ = CreateMenu();
            }
        }

        /// <summary>
        /// デストラクタ
        /// </summary>
        virtual ~MenuExtension() = default;


        /// <summary>
        /// ウィンドウメニュー
        /// </summary>
        HMENU WindowMenu_{};

        /// <summary>
        /// メニュー動作時関数マップ
        /// </summary>
        std::unordered_map<UINT, std::function<void(WPARAM, LPARAM)>> MenuFunctions_{};

        /// <summary>
        /// ウィンドウプロシージャ関数
        /// </summary>
        /// <param name="hWnd">ウィンドウハンドル</param>
        /// <param name="message">メッセージ</param>
        /// <param name="wParam">パラメーター</param>
        /// <param name="lParam">パラメーター</param>
        void ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override {

            if (MenuFunctions_.empty() || hWnd_ != hWnd) {
                return;
            }
            switch (message)
            {
            case WM_COMMAND:
            {
                int wmId = LOWORD(wParam);
                const auto it = MenuFunctions_.find(wmId);
                if (it != MenuFunctions_.end()) {
                    it->second(wParam, lParam);
                }
                break;
            }
            }
        };

        /// <summary>
        /// メニューバーセット関数
        /// </summary>
        /// <param name="hWnd">セットするウィンドウハンドル</param>
        /// <param name="hMenu">セットするメニュー</param>
        void SetMenuBar(HWND hWnd, HMENU hMenu) const {
            SetMenu(hWnd, hMenu);
            DrawMenuBar(hWnd);
        }

        bool AddMenuFunction(UINT ID, std::function<void(WPARAM, LPARAM)> func) {
            return MenuFunctions_.try_emplace(ID, func).second;
        }

        void CallMenuFunction(UINT ID,WPARAM wParam, LPARAM lParam) {

            const auto it = MenuFunctions_.find(ID);
            if (it != MenuFunctions_.end()) {
                it->second(wParam, lParam);
            }
        }

        void DeleteMenuFunction(UINT ID) {
            MenuFunctions_.erase(ID);
        }

        void ClearMenuFunction() {
            MenuFunctions_.clear();
        }

    };

    class MenuFactory
    {
    public:

        /* ===== メニュー本体作成 ===== */

        /// <summary>
        /// メニュー作成関数
        /// </summary>
        /// <param name="Menu">追加先メニュー</param>
        /// <param name="infos">追加するメニュー情報配列</param>
        /// <returns>作成の成否</returns>
        static bool CreateMenuToInfoVec(
            HMENU Menu, 
            std::initializer_list<MENUITEMINFOW> infos
        ) {

            for (auto& info : infos) {

                if (!InsertMenuItemW(Menu, GetMenuItemCount(Menu), TRUE, &info)) {
                    DestroyMenu(Menu);
                    return false;
                }
            }
            return true;
        }
        static bool CreateMenuToInfoVec(
            HMENU Menu, 
            std::span<MENUITEMINFOW> infos
        ) {

            for (auto& info : infos) {

                if (!InsertMenuItemW(Menu, GetMenuItemCount(Menu), TRUE, &info)) {
                    DestroyMenu(Menu);
                    return false;
                }
            }
            return true;
        }


        /// <summary>
        /// メニュー作成関数
        /// </summary>
        /// <param name="infos">追加するメニュー情報配列</param>
        /// <returns>作成の成否</returns>
        static HMENU CreateMenuToInfo(
            std::initializer_list<MENUITEMINFOW> infos
        ) {

            auto menu = CreateMenu();
            if (!menu) {
                return nullptr;
            }
            return CreateMenuToInfoVec(menu, infos) ? menu : nullptr;
        }
        static HMENU CreateMenuToInfo(
            std::span<MENUITEMINFOW> infos
        ) {

            auto menu = CreateMenu();
            if (!menu) {
                return nullptr;
            }
            return CreateMenuToInfoVec(menu, infos) ? menu : nullptr;
        }

        /// <summary>
        /// ポップアップメニュー作成関数
        /// </summary>
        /// <param name="infos">追加するメニュー情報配列</param>
        /// <returns>作成の成否</returns>
        static HMENU CreatePopupMenuToInfo(
            std::initializer_list<MENUITEMINFOW> infos
        ) {

            auto menu = CreatePopupMenu();
            if (!menu) {
                return nullptr;
            }
            return CreateMenuToInfoVec(menu, infos) ? menu : nullptr;
        }
        static HMENU CreatePopupMenuToInfo(
            std::span<MENUITEMINFOW> infos
        ) {

            auto menu = CreatePopupMenu();
            if (!menu) {
                return nullptr;
            }
            return CreateMenuToInfoVec(menu, infos) ? menu : nullptr;
        }

        /// <summary>
        /// メニュー情報追加関数
        /// </summary>
        /// <param name="Menu">追加先メニュー</param>
        /// <param name="info">追加する情報</param>
        /// <returns>追加の成否</returns>
        static bool AddMenuToInfo(
            HMENU Menu,
            MENUITEMINFOW info
        ) {

            if (!Menu) {
                return false;
            }

            return InsertMenuItemW(Menu, GetMenuItemCount(Menu), TRUE, &info) == TRUE;
        }


        /// <summary>
        /// メニュー追加関数
        /// </summary>
        /// <param name="mainMenu">追加先メニュー</param>
        /// <param name="subMenu">追加するメニュー</param>
        /// <param name="text">追加するメニュー名</param>
        /// <param name="position">追加するメニュー位置</param>
        /// <returns>追加の成否</returns>
        static bool AddSubMenu(
            HMENU mainMenu,
            HMENU subMenu,
            const wchar_t* text
        ) {

            if (!mainMenu || !subMenu || !text) {
                return false;
            }

            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_FTYPE;
            item.fType = MFT_STRING;
            item.dwTypeData = const_cast<LPWSTR>(text);
            item.hSubMenu = subMenu;

            return InsertMenuItemW(mainMenu, GetMenuItemCount(mainMenu), TRUE, &item) == TRUE;
        }


        /// <summary>
        /// メニュー追加関数
        /// </summary>
        /// <details>
        /// 追加するメニューの位置を指定する
        /// </details>
        /// <param name="mainMenu">追加先メニュー</param>
        /// <param name="subMenu">追加するメニュー</param>
        /// <param name="text">追加するメニュー名</param>
        /// <param name="position">追加するメニュー位置</param>
        /// <returns>追加の成否</returns>
        static bool AddSubMenu(
            HMENU mainMenu,
            HMENU subMenu,
            const wchar_t* text,
            UINT position
        ) {

            if (!mainMenu || !subMenu || !text) {
                return false;
            }

            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_FTYPE;
            item.fType = MFT_STRING;
            item.dwTypeData = const_cast<LPWSTR>(text);
            item.hSubMenu = subMenu;

            return InsertMenuItemW(mainMenu, position, TRUE, &item) == TRUE;
        }

        /// <summary>
        /// メニュー追加関数
        /// </summary>
         /// <details>
        /// 追加するメニューを情報から作成
        /// </details>
        /// <param name="mainMenu">追加先メニュー</param>
        /// <param name="text">追加するメニュー名</param>
        /// <param name="infos">追加する情報</param>
        /// <returns>追加の成否</returns>
        static bool AddSubMenu(
            HMENU mainMenu,
            const wchar_t* text,
            std::initializer_list<MENUITEMINFOW> infos
        ) {

            if (!mainMenu || !text) {
                return false;
            }

            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_FTYPE;
            item.fType = MFT_STRING;
            item.dwTypeData = const_cast<LPWSTR>(text);
            item.hSubMenu = CreatePopupMenuToInfo(infos);

            return InsertMenuItemW(mainMenu, GetMenuItemCount(mainMenu), TRUE, &item) == TRUE;
        }
        static bool AddSubMenu(
            HMENU mainMenu,
            const wchar_t* text,
            std::span<MENUITEMINFOW> infos
        ) {

            if (!mainMenu || !text) {
                return false;
            }

            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_FTYPE;
            item.fType = MFT_STRING;
            item.dwTypeData = const_cast<LPWSTR>(text);
            item.hSubMenu = CreatePopupMenuToInfo(infos);

            return InsertMenuItemW(mainMenu, GetMenuItemCount(mainMenu), TRUE, &item) == TRUE;
        }

        /// <summary>
        /// メニュー情報セット関数
        /// </summary>
        /// <param name="hWnd">セットするメニューがあるウィンドウハンドル</param>
        /// <param name="menu">セットするメニュー</param>
        /// <param name="mask">セットするマスク情報</param>
        /// <param name="state">セットするステート</param>
        /// <param name="id">セットするID情報(識別ID)</param>
        /// <returns>セットの成否</returns>
        static bool SetMenuToInfo(
            HWND hWnd,
            HMENU menu,
            UINT mask,
            UINT state,
            UINT id
        ) {

            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = mask;
            item.fState = state;

            if (!SetMenuItemInfoW(menu, id, FALSE, &item)) {
                return false;
            }

            return true;
        }

        /* ===== メニュー情報作成 ===== */

        static MENUITEMINFOW MakeMenuItem(
            UINT id,
            LPCWSTR text,
            UINT state = MFS_ENABLED
        ) {
            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = MIIM_ID | MIIM_STRING | MIIM_FTYPE | MIIM_STATE;
            item.fType = MFT_STRING;
            item.fState = state;
            item.wID = id;
            item.dwTypeData = const_cast<LPWSTR>(text);

            return item;
        }

        static MENUITEMINFOW MakeSubMenuItem(
            HMENU subMenu,
            LPCWSTR text,
            UINT state = MFS_ENABLED
        ) {
            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = MIIM_SUBMENU | MIIM_STRING | MIIM_FTYPE | MIIM_STATE;
            item.fType = MFT_STRING;
            item.fState = state;
            item.hSubMenu = subMenu;
            item.dwTypeData = const_cast<LPWSTR>(text);

            return item;
        }


        // 区切り線
        static MENUITEMINFOW MakeMenuSeparator() {
            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = MIIM_FTYPE;
            item.fType = MFT_SEPARATOR;

            return item;
        }


        static MENUITEMINFOW MakeMenuItemToInfo(
            UINT mask,
            UINT type = MFT_STRING,
            UINT state = MFS_ENABLED,
            UINT id = 0,
            LPCWSTR text = nullptr,
            HMENU subMenu = nullptr
        ) {
            MENUITEMINFOW item{};
            item.cbSize = sizeof(item);
            item.fMask = mask;
            item.fType = type;
            item.fState = state;
            item.wID = id;
            item.dwTypeData = const_cast<LPWSTR>(text);
            item.hSubMenu = subMenu;

            return item;
        }


    private:
        MenuFactory() = default;
        ~MenuFactory() = default;
    };



}
