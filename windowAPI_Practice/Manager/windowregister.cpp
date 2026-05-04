#include"windowregister.h"

void windowregister::submit_class(ID::class_id id, class_params param) {

	WNDCLASSEX wc{};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = param.proc;
	wc.hInstance = param.hinstance;
	wc.lpszClassName = param.name.c_str();
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	if (!RegisterClassEx(&wc)) {
		if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {

		}
		return;
	}

	class_map.try_emplace(id, param.name);
}

[[nodiscard]] std::optional<std::wstring> windowregister::get_class(ID::class_id id)const noexcept {
	auto it = class_map.find(id);
	if (it == class_map.end()) {
		return std::nullopt;
	}

	return it->second;
}