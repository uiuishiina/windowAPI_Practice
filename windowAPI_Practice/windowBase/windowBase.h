#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<functional>

class windowManager;

class windowBase
{
protected:
	friend windowManager;
	HWND hwnd_{};
	std::function<void()> callback_{};

	virtual LRESULT window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	virtual ~windowBase() {
		OutputDebugStringA("onDestroywindow\n");
	}

	[[nodiscard]] virtual bool create_window(HINSTANCE hInstance, WNDCLASSEX ex) = 0;

	[[nodiscard]] HWND get_hwnd()const noexcept;
};