#pragma once

#include"framework.h"
#include "windowAPI.h"

class ApplicationExtensionBase
{
public:

	ApplicationExtensionBase(HWND hWnd) :
		hWnd_{ hWnd } {}

	virtual ~ApplicationExtensionBase() = default;

	virtual LRESULT ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { 
		return DefWindowProc(hWnd, message, wParam, lParam); 
	};

	void ShowMessage(LPCUWSTR mainText, LPCUWSTR titleText) const {
		MessageBoxW(
			nullptr,
			mainText,
			titleText,
			MB_OK
		);
	}

	bool YesNoMessage(LPCUWSTR mainText, LPCUWSTR titleText) const {
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

};