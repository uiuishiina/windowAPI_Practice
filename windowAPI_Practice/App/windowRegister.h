#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<unordered_map>
#include<optional>

enum class windowclassID {
	Main,Sub
};

class windowRegister final
{
	std::unordered_map<windowclassID, WNDCLASSEX> class_map{};
public:
	void register_class(windowclassID id, WNDCLASSEX class_ex);

	[[nodiscard]] std::optional<WNDCLASSEX> get_class(windowclassID id)const noexcept;
};