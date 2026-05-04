#pragma once
#include"windowBase.h"

class windowSub final :public windowBase
{
public:
	[[nodiscard]] bool create_window(HINSTANCE hInstance, std::wstring& class_name)override;
};