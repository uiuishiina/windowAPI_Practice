#pragma once
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<unordered_map>
#include<optional>
#include<string>

namespace ID {
	enum class class_id {
		main, sub
	};

	inline std::wstring to_string_ID(class_id id) {
		std::wstring str{};
		switch (id)
		{
		case ID::class_id::main:
			str = L"main_window";
			break;
		case ID::class_id::sub:
			str = L"sub_window";
			break;
		default:
			break;
		}
		return str;
	}
}

struct class_params {
	std::wstring name;
	WNDPROC proc;
	HINSTANCE hinstance;
};

class windowregister final
{
	std::unordered_map<ID::class_id, std::wstring> class_map{};
public:
	void submit_class(ID::class_id id, class_params param);

	[[nodiscard]] std::optional<std::wstring> get_class(ID::class_id id)const noexcept;
};