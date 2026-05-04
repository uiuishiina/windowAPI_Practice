#include"windowManager.h"
#include"../windowBase/windowMain.h"
#include"../windowBase/windowSub.h"
//---------------------------------------------------------------------------------------------------

LRESULT CALLBACK windowManager::StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto pWnd = reinterpret_cast<windowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (msg == WM_NCCREATE) {
		auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pWnd = reinterpret_cast<windowBase*>(cs->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		return pWnd->window_proc(hwnd, msg, wParam, lParam);
	}

	if (msg == WM_NCDESTROY) {
		if (pWnd) {
			SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
			auto result = pWnd->window_proc(hwnd, msg, wParam, lParam);
			pWnd->hwnd_ = nullptr;
			return result;
		}
		return 0;
	}

	if (pWnd) {
		return pWnd->window_proc(hwnd, msg, wParam, lParam);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

[[nodiscard]] std::wstring windowManager::get_classname(ID::class_id id) {
	if (auto c = register_->get_class(id); c.has_value()) {
		return c.value();
	}

	class_params param{};
	param.name = ID::to_string_ID(id);
	param.hinstance = hInstance_;
	param.proc = StaticWindowProc;

	register_->submit_class(id, param);

	return register_->get_class(id).value();
}

[[nodiscard]] std::optional<std::unique_ptr<windowBase>> windowManager::create_window_instance(ID::class_id id)
{
	auto it = factory_map.find(id);
	if (it == factory_map.end()) {
		return nullptr;
	}

	auto window = it->second();
	auto class_name = get_classname(id);

	if (!window->create_window(hInstance_, class_name)) {
		return std::nullopt;
	}

	return window;
}

//---------------------------------------------------------------------------------------------------

[[nodiscard]] bool windowManager::initalize(HINSTANCE hInstance) {
	register_ = std::make_unique<windowregister>();
	hInstance_ = hInstance;
	factory_map[ID::class_id::main] = [] {
		return std::make_unique<windowMain>();
		};
	factory_map[ID::class_id::sub] = [] {
		return std::make_unique<windowSub>();
		};

	return true;
}

//---------------------------------------------------------------------------------------------------

[[nodiscard]] bool windowManager::create_window(ID::class_id id) {

	auto window = create_window_instance(id);
	if (!window.has_value()) {
		return false;
	}

	auto hwnd = window.value()->get_hwnd();
	window.value()->ondestroy_callback = [this, hwnd]() {this->onDestroy_window(hwnd); };

	window_map.emplace(hwnd, std::move(window.value()));

	return true;
}

//---------------------------------------------------------------------------------------------------

[[nodiscard]] bool windowManager::message_loop() {

	MSG msg_{};
	while (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE)) {
		if (msg_.message == WM_QUIT) {
			return false;
		}
		TranslateMessage(&msg_);
		DispatchMessage(&msg_);
	}

	return true;
}

void windowManager::onDestroy_window(HWND hwnd) {
	auto it = window_map.find(hwnd);
	if (it != window_map.end()) {
		window_map.erase(it);
	}
}