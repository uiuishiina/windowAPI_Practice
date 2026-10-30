#pragma once

#include"TimerBaseExtension.h"
#include"SubWindow.h"
#include"MenuExtension.h"

#include<memory>

namespace AudioCapture {

	constexpr UINT AudioTimerID = 16000;

	enum AudioCaptureID : UINT {
		IDM_EXT_CREATE_AUDIO_CAPTURE_WINDOW = 16001
	};

	class AudioCapture : public ApplicationExtensionBase
	{
	public:

		explicit AudioCapture(HWND hWnd, HINSTANCE hInstance) :
			ApplicationExtensionBase{ hWnd, hInstance },
			AppMenu_{ std::make_unique<MenuExt::MenuExtension>(hWnd,hInstance)} {

			AudioCaptureWindow_ = std::make_unique<SubWindow::NormalSubwindow>(
				hInstance,
				L"AudioCapture"
			);
			
			MenuExt::MenuFactory::AddSubMenu(
				AppMenu_->WindowMenu_,
				L"AudioCapture",
				{
					MenuExt::MenuFactory::MakeMenuItem(
					IDM_EXT_CREATE_AUDIO_CAPTURE_WINDOW,
					L"AudioCapture ウィンドウ作成"
				)
				}
			);

			AppMenu_->AddMenuFunction(
				IDM_EXT_CREATE_AUDIO_CAPTURE_WINDOW, 
				[this](WPARAM wParam, LPARAM lParam) {

					if (!AudioCaptureWindow_) {
						return;
					}
					if (AudioCaptureWindow_->GetSubWindow()) {
						ShowWindow(AudioCaptureWindow_->GetSubWindow(), SW_SHOW);
						SetForegroundWindow(AudioCaptureWindow_->GetSubWindow());
						return;
					}
					if (AudioCaptureWindow_->CreateNormalSubWindow(1000, 700)) {

						NormalTimer_ = std::make_unique<TimerExtension::NormalTimerExtension>(
							AudioCaptureWindow_->GetSubWindow(),
							hInstance_,
							60,
							AudioTimerID
						);
						NormalTimer_->StartTimer();
						NormalTimer_->AddTimerCallback(
							[this](WPARAM wParam, LPARAM lParam) {
								InvalidateRect(AudioCaptureWindow_->GetSubWindow(), nullptr, FALSE);
							}
						);
						AudioCaptureWindow_->AddExtensionVec(NormalTimer_.get());
						AudioCaptureWindow_->AddProcFuncVec(
							[this](UINT message, WPARAM wParam, LPARAM lParam) {

								if (message == WM_PAINT) {
									PAINTSTRUCT ps;

									auto hwnd = AudioCaptureWindow_->GetSubWindow();
									auto hdc = BeginPaint(hwnd, &ps);

									RECT client;
									GetClientRect(hwnd, &client);

									int width = client.right;
									int height = client.bottom;

									HDC mem = CreateCompatibleDC(hdc);
									HBITMAP bmp = CreateCompatibleBitmap(
										hdc, std::max(1, width), std::max(1, height));

									HGDIOBJ oldBmp = SelectObject(mem, bmp);

									SetBackGround(mem, client);

									SetBkMode(mem, TRANSPARENT);

									BitBlt(
										hdc,
										0, 0,
										width, height,
										mem,
										0, 0,
										SRCCOPY
									);

									SelectObject(mem, oldBmp);
									DeleteObject(bmp);
									DeleteDC(mem);

									EndPaint(AudioCaptureWindow_->GetSubWindow(), &ps);
								}
							});

						AudioCaptureWindow_->SetOnDestroyFunc(
							[this](WPARAM wParam, LPARAM lParam) {
								if (NormalTimer_) {
									NormalTimer_->StopTimer();
								}
								AudioCaptureWindow_->ClearAll();
							});
					}

				});

			AppMenu_->SetMenuBar(hWnd_, AppMenu_->WindowMenu_);
		};

		void ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override {

			if (AppMenu_) {
				AppMenu_->ExtensionProc(hWnd, message, wParam, lParam);
			}
		};


	private:

		std::unique_ptr<MenuExt::MenuExtension> AppMenu_{};

		std::unique_ptr<SubWindow::NormalSubwindow> AudioCaptureWindow_{};

		std::unique_ptr<TimerExtension::NormalTimerExtension> NormalTimer_{};

		void SetBackGround(HDC& mem, RECT rect) {

			HBRUSH bg = CreateSolidBrush(RGB(16, 21, 28));
			FillRect(mem, &rect, bg);
			DeleteObject(bg);
		}

	};
}