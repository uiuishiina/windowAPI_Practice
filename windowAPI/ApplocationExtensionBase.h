#pragma once

#include"framework.h"
#include "windowAPI.h"
#include<span>


class ApplicationExtensionBase
{
public:

	ApplicationExtensionBase(HWND hWnd, HINSTANCE hInstance) :
		hWnd_{ hWnd }, hInstance_{ hInstance } {}

	virtual ~ApplicationExtensionBase() = default;

	virtual void ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {};

	HMENU CreateMenuToInfo( 
		UINT mask, 
		UINT type,
		UINT state,
		UINT id, 
		const wchar_t* text, 
		UINT position
	) {

		auto menu = CreateMenu();
		if (!menu) {
			return nullptr;
		}

		MENUITEMINFOW item{};
		item.cbSize = sizeof(item);
		item.fMask = mask;
		item.fType = type;
		item.fState = state;
		item.wID = id;
		item.dwTypeData = const_cast<LPWSTR>(text);

		if (InsertMenuItemW(menu, position, TRUE, &item)) {
			DestroyMenu(menu);
			return nullptr;
		}
		
		return menu;
	}

	HMENU CreatePopupMenuToInfo(
		UINT mask,
		UINT type,
		UINT state,
		UINT id,
		const wchar_t* text,
		UINT position
	) {

		auto menu = CreatePopupMenu();
		if (!menu) {
			return nullptr;
		}

		MENUITEMINFOW item{};
		item.cbSize = sizeof(item);
		item.fMask = mask;
		item.fType = type;
		item.fState = state;
		item.wID = id;
		item.dwTypeData = const_cast<LPWSTR>(text);

		if (InsertMenuItemW(menu, position, TRUE, &item)) {
			DestroyMenu(menu);
			return nullptr;
		}

		return menu;
	}

	bool AddSubMenu(
		HMENU mainMenu,
		HMENU subMenu,
		UINT mask, 
		const wchar_t* text, 
		UINT position
	) {

		if (!mainMenu || !subMenu || !text) {
			return false;
		}

		MENUITEMINFOW item{};
		item.cbSize = sizeof(item);
		item.fMask = mask | MIIM_SUBMENU;
		item.dwTypeData = const_cast<LPWSTR>(text);
		item.hSubMenu = subMenu;

		return InsertMenuItemW(mainMenu, position, TRUE, &item) == TRUE;
	}

	bool SetMenuToInfo(
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

		DrawMenuBar(hWnd);

		return true;
	}

	void SetMenuBar(HWND hWnd, HMENU hMenu) const {
		SetMenu(hWnd, hMenu);
		DrawMenuBar(hWnd);
	}

	void ShowMessage(LPCWSTR mainText, LPCWSTR titleText) const {
		MessageBoxW(
			nullptr,
			mainText,
			titleText,
			MB_OK
		);
	}

	void ShowMessage(LPCSTR mainText, LPCSTR titleText) const {
		MessageBoxA(
			nullptr,
			mainText,
			titleText,
			MB_OK
		);
	}

	bool YesNoMessage(LPCWSTR mainText, LPCWSTR titleText) const {
		int result = MessageBoxW(
			hWnd_,
			mainText,
			titleText,
			MB_YESNO
		);

		return result == IDYES ? true : false;
	}

protected:

	HWND hWnd_{};
	HINSTANCE hInstance_{};

	HMENU RootMenu_{};
};