#pragma once

#include"framework.h"
#include"Resource.h"

//	アプリケーション拡張機能基底クラス
class ApplicationExtensionBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="hWnd">メインアプリケーションウィンドウハンドル</param>
	/// <param name="hInstance">ウィンドウインスタンス</param>
	ApplicationExtensionBase(HWND hWnd, HINSTANCE hInstance) :
		hWnd_{ hWnd }, hInstance_{ hInstance } {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ApplicationExtensionBase() = default;

	/// <summary>
	/// 拡張機能用ウィンドウプロシージャ純粋仮想関数
	/// </summary>
	/// <details>
	/// メインウィンドウプロシージャから呼び出される
	/// </details>
	/// <param name="hWnd">メインアプリケーションウィンドウハンドル</param>
	/// <param name="message">メッセージ</param>
	/// <param name="wParam">パラメーター</param>
	/// <param name="lParam">パラメーター</param>
	virtual void ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {};

	/// <summary>
	/// メッセージ表示機能
	/// </summary>
	/// <details>
	/// 16バイト文字列用
	/// </details>
	/// <param name="mainText">メインテキスト</param>
	/// <param name="titleText">タイトルテキスト</param>
	void ShowMessage(LPCWSTR mainText, LPCWSTR titleText) const {
		MessageBoxW(
			nullptr,
			mainText,
			titleText,
			MB_OK
		);
	}

	/// <summary>
	/// メッセージ表示機能
	/// </summary>
	/// <details>
	/// 8バイト文字列用
	/// </details>
	/// <param name="mainText">メインテキスト</param>
	/// <param name="titleText">タイトルテキスト</param>
	void ShowMessage(LPCSTR mainText, LPCSTR titleText) const {
		MessageBoxA(
			nullptr,
			mainText,
			titleText,
			MB_OK
		);
	}

	/// <summary>
	/// 確認メッセージ表示機能
	/// </summary>
	/// <param name="mainText">メインテキスト</param>
	/// <param name="titleText">タイトルテキスト</param>
	/// <returns>Yes なら [ true ] | No なら [ false ]</returns>
	bool YesNoMessage(LPCWSTR mainText, LPCWSTR titleText) const {
		int result = MessageBoxW(
			hWnd_,
			mainText,
			titleText,
			MB_YESNO
		);

		return result == IDYES ? true : false;
	}

protected:

	/// <summary>
	/// メインアプリケーションウィンドウハンドル
	/// </summary>
	HWND hWnd_{};

	/// <summary>
	/// ウィンドウインスタンス
	/// </summary>
	HINSTANCE hInstance_{};
};
