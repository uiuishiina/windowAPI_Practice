#pragma once

#include"TimerBaseExtension.h"

#include<chrono>
#include<cstdint>

namespace TimerExtension {

	class CorrectedTimerExtension : public TimerBaseExtension
	{
	public:
		explicit CorrectedTimerExtension(
			HWND hWnd, 
			HINSTANCE hInstance, 
			UINT interval, 
			UINT timerId,
			bool start = false
		) :
			TimerBaseExtension{ hWnd, hInstance, interval, timerId, start } {

			last_ = std::chrono::steady_clock::now();
		}

	protected:

		void OnTimer(
			WPARAM wParam, 
			LPARAM lParam
		) override {

			auto now = std::chrono::steady_clock::now();

			auto elapsed = now - last_;

			last_ = now;

			const auto interval = std::chrono::milliseconds{ IntervalMs_ };

			duration_ += elapsed;

			auto count = duration_ / interval;

			duration_ %= interval;

			for (decltype(count) i = 0; i < count; ++i) {
				CallAllFunc(wParam, lParam);
			}
		}

	private:

		std::chrono::steady_clock::time_point last_{};

		std::chrono::steady_clock::duration duration_{};

	};
}