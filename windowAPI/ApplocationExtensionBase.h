#pragma once

#include"framework.h"
#include "windowAPI.h"

class ApplicationExtensionBase
{
public:

	ApplicationExtensionBase(HWND hWnd, HINSTANCE hInstance) :
		hWnd_{ hWnd }, hInstance_{ hInstance } {}

	virtual ~ApplicationExtensionBase() = default;

	virtual void ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {};

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

};