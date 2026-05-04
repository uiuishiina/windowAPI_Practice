#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<functional>
#include<string>

class windowManager;

class windowBase
{
protected:
	friend windowManager;
	HWND	hwnd_{};
	std::function<void()> ondestroy_callback{};

	virtual LRESULT window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	windowBase() = default;
	virtual ~windowBase() = default;

	[[nodiscard]] virtual bool create_window(HINSTANCE hInstance, std::wstring& class_name) = 0;

	[[nodiscard]] HWND get_hwnd() const noexcept;
};