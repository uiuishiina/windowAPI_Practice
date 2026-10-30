#pragma once

#include"../ApplocationExtensionBase.h"

#include<functional>

namespace TimerExtension {

	/// <summary>
	/// タイマー機能拡張
	/// </summary>
	class TimerBaseExtension : public ApplicationExtensionBase
	{
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="hWnd">ウィンドウハンドル</param>
		/// <param name="hInstance">ウィンドウインスタンス</param>
		explicit TimerBaseExtension(
			HWND hWnd, 
			HINSTANCE hInstance, 
			UINT interval, 
			UINT timerId,
			bool start = false
		) :
			ApplicationExtensionBase{ hWnd, hInstance },
			IntervalMs_{ interval },
			TimerId_{ timerId } {
			
			if (start) {
				StartTimer();
			}
		};

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~TimerBaseExtension() {

			StopTimer();
		}


		void ExtensionProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		) override {
			
			if (TimeCallFunc_.empty() || hWnd_ != hWnd) {
				return;
			}
			if (message == WM_TIMER) {
				OnTimer(wParam, lParam);
			}
		}

		void SetTimerInterval(
			UINT interval,
			UINT timerId
		) {

			IntervalMs_ = interval;
			TimerId_ = timerId;
		}

		void AddTimerCallback(
			std::function<void(WPARAM, LPARAM)> func
		) {
			TimeCallFunc_.push_back(std::move(func));
		}

		bool StartTimer() {

			if (running_) {
				return false;
			}

			if (SetTimer(hWnd_, TimerId_, IntervalMs_, nullptr) == 0) {
				return false;
			}
			running_ = true;

			return true;
		}

		bool StopTimer() {

			if (!running_) {
				return false;
			}
			if (KillTimer(hWnd_, TimerId_)) {
				running_ = false;

				return true;
			}
			else {
				return false;
			}
		}

	protected:

		virtual void OnTimer(WPARAM wParam, LPARAM lParam) = 0;

		void CallAllFunc(
			WPARAM wParam, 
			LPARAM lParam
		) {

			for (auto& func : TimeCallFunc_) {
				func(wParam, lParam);
			}
		}
		
		/// <summary>
		/// ミリ秒
		/// </summary>
		UINT IntervalMs_{};

		UINT TimerId_{};

		bool running_ = false;

		std::vector<std::function<void(WPARAM, LPARAM)>> TimeCallFunc_{};

	};

	class NormalTimerExtension : public TimerBaseExtension
	{
	public:

		using TimerBaseExtension::TimerBaseExtension;

	protected:

		void OnTimer(
			WPARAM wParam, 
			LPARAM lParam
		) override {
			CallAllFunc(wParam, lParam);
		}
	};

}