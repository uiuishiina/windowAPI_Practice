#pragma once
#include"../windowBase/windowBase.h"
#include"windowregister.h"
#include<memory>
#include<unordered_map>

class windowManager final
{
	HINSTANCE	hInstance_{};
	std::unique_ptr<windowregister> register_{};
	std::unordered_map<HWND, std::unique_ptr<windowBase>> window_map{};
	std::unordered_map<ID::class_id, std::function<std::unique_ptr<windowBase>()>> factory_map;

	static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	[[nodiscard]] std::wstring get_classname(ID::class_id id);

	[[nodiscard]] std::optional<std::unique_ptr<windowBase>> create_window_instance(ID::class_id id);
public:
	[[nodiscard]] bool initalize(HINSTANCE hInstance);

	[[nodiscard]] bool create_window(ID::class_id id);

	[[nodiscard]] bool message_loop();

	void onDestroy_window(HWND hwnd);
};