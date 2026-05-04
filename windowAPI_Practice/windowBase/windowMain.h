#pragma once
#include"windowBase.h"

class windowMain final :public windowBase
{
	LRESULT window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
public:
	[[nodiscard]] bool create_window(HINSTANCE hInstance, std::wstring& class_name)override;
};