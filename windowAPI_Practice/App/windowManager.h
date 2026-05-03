#pragma once
#include"../windowBase/windowBase.h"
#include"windowRegister.h"
#include<memory>

class windowManager final
{
	HINSTANCE hInstance_{};

	std::unordered_map<HWND,std::unique_ptr<windowBase>> sub_window{};

	std::unique_ptr<windowRegister> register_{};

	static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void register_submit(windowclassID id);
public:
	[[nodiscard]] bool initalize(HINSTANCE hInstance);

	[[nodiscard]] bool create_window(windowclassID id);

	[[nodiscard]] bool message_loop();

	void ondestroy_window(HWND hwnd);

	void end();
};