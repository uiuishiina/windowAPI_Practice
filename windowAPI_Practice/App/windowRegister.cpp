#include"windowRegister.h"

void windowRegister::register_class(windowclassID id, WNDCLASSEX class_ex) {
	if (!RegisterClassEx(&class_ex)) {
		return;
	}

	class_map.try_emplace(id, class_ex);
}

[[nodiscard]] std::optional<WNDCLASSEX> windowRegister::get_class(windowclassID id)const noexcept {

	auto it = class_map.find(id);
	if (it == class_map.end()) {
		return std::nullopt;
	}

	return it->second;
}