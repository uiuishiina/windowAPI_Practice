#pragma once

#include"../framework.h"
#include"../Resource.h"

#include<string>
#include<functional>

namespace SubWindow {

	class SubWindow
	{
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
        SubWindow(std::wstring ClassName) :
            ClassName_{ ClassName } {}

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~SubWindow() = default;

        HWND GetSubWindow() const noexcept {
            return SubhWnd_;
        }

	protected:

        /// <summary>
        /// サブウィンドウハンドル
        /// </summary>
        HWND SubhWnd_{};

        /// <summary>
        /// ウィンドウクラス名
        /// </summary>
        /// <details>
        /// 継承先でそれぞれ定義
        /// </details>
        std::wstring ClassName_{};

		/// <summary>
		/// ウィンドウクラス作成関数
		/// </summary>
		/// <returns>作成したウィンドウクラス</returns>
		virtual const WNDCLASSEXW CreateWindowClass() const noexcept = 0;

        /// <summary>
        /// サブウィンドウプロシージャ関数
        /// </summary>
        /// <param name="hWnd">ウィンドウハンドル</param>
        /// <param name="message">メッセージ</param>
        /// <param name="wParam">パラメーター</param>
        /// <param name="lParam">パラメーター</param>
        /// <returns>メッセージの返値</returns>
        virtual LRESULT SubWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

		/// <summary>
		/// ウィンドウプロシージャ関数
		/// </summary>
        /// <param name="hWnd">ウィンドウハンドル</param>
        /// <param name="message">メッセージ</param>
        /// <param name="wParam">パラメーター</param>
        /// <param name="lParam">パラメーター</param>
        /// <returns>メッセージの返値</returns>
		LRESULT static CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

            /* ===== 初期化時 ===== */
            if (message == WM_NCCREATE) {

                auto cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                auto pWnd = reinterpret_cast<SubWindow*>(cs->lpCreateParams);

                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

                return TRUE;
            }

            if (auto pWnd = reinterpret_cast<SubWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA)); pWnd) {

                /* ===== 最終破棄時 ===== */
                if (message == WM_NCDESTROY) {
                    SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
                }
                /* ===== 継承先処理 ===== */
                return pWnd->SubWindowProc(hWnd, message, wParam, lParam);
            }

            return DefWindowProc(hWnd, message, wParam, lParam);
		}

        /// <summary>
        /// サブウィンドウ作成関数
        /// </summary>
        /// <param name="hInstance">ウィンドウインスタンス</param>
        /// <param name="width">横幅</param>
        /// <param name="height">縦幅</param>
        /// <returns>作成の成否</returns>
        bool CreateSubWindow(HINSTANCE hInstance,unsigned int width, unsigned int height) {

            SubhWnd_ = CreateWindowExW(
                0,
                ClassName_.c_str(),
                ClassName_.c_str(),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                nullptr,
                nullptr,
                hInstance,
                this
            );
            if (!SubhWnd_) {
                return false;
            }

            SetWindowTextW(SubhWnd_, ClassName_.c_str());
            ShowWindow(SubhWnd_, SW_SHOW);
            UpdateWindow(SubhWnd_);
            SetForegroundWindow(SubhWnd_);

            return true;
        }



	};

    class NormalSubwindow : public SubWindow
    {
    public:

        explicit NormalSubwindow(HINSTANCE hInstace, std::wstring ClassName) :
            SubWindow{ ClassName },
            hInstance_{ hInstace } {
            
            //  ウィンドウクラス登録
            const auto wc = CreateWindowClass();
            RegisterClassExW(&wc);
        }

        /// <summary>
        /// サブウィンドウプロシージャ関数
        /// </summary>
        /// <param name="hWnd">ウィンドウハンドル</param>
        /// <param name="message">メッセージ</param>
        /// <param name="wParam">パラメーター</param>
        /// <param name="lParam">パラメーター</param>
        /// <returns>メッセージの返値</returns>
        LRESULT SubWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override {
            
            switch (message)
            {
            case WM_CLOSE:
                DestroyWindow(hWnd);
                return 0;

            case WM_DESTROY:
                OnDestroySubWindow();
                if (!OnDestroyFunc_.empty()) {
                    for (auto& func : OnDestroyFunc_) {
                        func(wParam, lParam);
                    }
                }
                return 0;
            default:
                break;
            }
            if (!ProcFunc_.empty()) {
                for (auto& func : ProcFunc_) {
                    func(message, wParam, lParam);
                }
            }
            if (!ExtensionVec_.empty()) {
                for (auto& Ext : ExtensionVec_) {
                    Ext->ExtensionProc(hWnd, message, wParam, lParam);
                }
            }

            return DefWindowProcW(
                hWnd, message, wParam, lParam
            );
        };

       
        bool CreateNormalSubWindow(unsigned int width, unsigned int height) {
            return CreateSubWindow(hInstance_, width, height);
        }

        void OnDestroySubWindow() {

            SubhWnd_ = nullptr;
        }

        void SetOnDestroyFunc(
            std::function<void(WPARAM, LPARAM)> func
        ) {
            OnDestroyFunc_.push_back(func);
        }
        void AddProcFuncVec(
            std::function<void(UINT, WPARAM, LPARAM)> func
        ) {
            ProcFunc_.push_back(func);
        }

        void AddExtensionVec(ApplicationExtensionBase* ext) {
            if (!ext) {
                return;
            }
            ExtensionVec_.push_back(ext);
        }

        void ClearOnDestroyFunc() {
            OnDestroyFunc_.clear();
        }
        void ClearProcFuncVec() {
            ProcFunc_.clear();
        }
        void ClearExtensionVec() {
            ExtensionVec_.clear();
        }
        void ClearAll() {
            ClearOnDestroyFunc();
            ClearProcFuncVec();
            ClearExtensionVec();
        }

    private:

        HINSTANCE hInstance_{};

        std::vector<std::function<void(WPARAM, LPARAM)>> OnDestroyFunc_{};

        std::vector<std::function<void(UINT,WPARAM, LPARAM)>> ProcFunc_{};

        std::vector<ApplicationExtensionBase*> ExtensionVec_{};

        /// <summary>
        /// ウィンドウクラス作成関数
        /// </summary>
        /// <returns>作成したウィンドウクラス</returns>
        const WNDCLASSEXW CreateWindowClass() const noexcept {

            WNDCLASSEXW wc{};
            wc.cbSize = sizeof(wc);
            wc.lpfnWndProc = WndProc;
            wc.hInstance = hInstance_;
            wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
            wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
            wc.lpszClassName = ClassName_.c_str();

            return wc;
        }

    };
}