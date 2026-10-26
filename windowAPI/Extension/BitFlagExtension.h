#pragma once

#include"../ApplocationExtensionBase.h"
#include"../Extension/MenuExtension.h"

#include"BitFlag.h"

#include<unordered_map>

namespace BitFlagExtension {

	namespace Concepts {

		template<typename T>
		concept EnumHasNone = std::is_enum_v <T>
			&& requires { T::None; };

		template<typename T>
		concept EnumHasAll = std::is_enum_v <T>
			&& requires { T::All; };

		template<HandyItem::Enum::concepts::IsEnum T>
		constexpr bool EnumHasNoneCheck() {
			return requires {
				T::None;
			};
		}
		template<HandyItem::Enum::concepts::IsEnum T>
		constexpr bool EnumHasAllCheck() {
			return requires {
				T::All;
			};
		}
	}

	/// <summary>
	/// ビットフラグ機能拡張
	/// </summary>
	/// <typeparam name="T">ビットフラグ化する列挙体</typeparam>
	template<HandyItem::Enum::concepts::EnumHasCount T>
	class BitFlagExtension : ApplicationExtensionBase
	{
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		explicit BitFlagExtension(HWND hWnd, HINSTANCE hInstance) :
			ApplicationExtensionBase{ hWnd ,hInstance } {}

		/// <summary>
		/// デストラクタ
		/// </summary>
		~BitFlagExtension() = default;

		void ExtensionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override {};

		/// <summary>
		/// フラグ切り替え関数
		/// </summary>
		/// <param name="value">切り替える値</param>
		void Toggle(T value) {
			Flag_.toggle(value);
		}

		/// <summary>
		/// 全フラグ [ True ] 関数
		/// </summary>
		void SetFlag() {
			Flag_.set_all();
		}

		/// <summary>
		/// フラグ [ True ] 関数
		/// </summary>
		/// <param name="value">[ True ] にする値</param>
		void SetFlag(T value) {
			Flag_.set(value);
		}

		/// <summary>
		/// 全フラグ [ False ] 関数
		/// </summary>
		void ResetFlag() {
			Flag_.set_none();
		}

		/// <summary>
		/// フラグ [ False ] 関数
		/// </summary>
		/// <param name="value">[ False ] にする値</param>
		void ResetFlag(T value) {
			Flag_.reset(value);
		}

		/// <summary>
		/// フラグテスト関数
		/// </summary>
		/// <param name="value">テストするフラグ</param>
		/// <returns>立っているなら [ True ]</returns>
		bool Test(T value) const {
			return Flag_.test(value);
		}

		/// <summary>
		/// 全フラグ [ False ] 判定関数
		/// </summary>
		/// <returns>すべて [ False ] なら [ True ]</returns>
		bool IsNone() const {
			return Flag_.none();
		}

		/// <summary>
		/// 全フラグ [ True ] 判定関数
		/// </summary>
		/// <returns>すべて [ True ] なら [ True ]</returns>
		bool IsAll() const {
			return Flag_.all();
		}

		/// <summary>
		/// 全フラグテスト関数
		/// </summary>
		/// <returns>いずれかが立っているなら [ True ]</returns>
		bool IsAny() const {
			return Flag_.any();
		}

		/// <summary>
		/// 指定範囲フラグ [ False ] 判定関数
		/// </summary>
		/// <param name="end">判定終了位置</param>
		/// <param name="start">判定開始位置</param>
		/// <returns>すべて [ False ] なら [ True ]</returns>
		bool RangeNone(T end, T start = T{}) const {
			for (
				auto i = HandyItem::Enum::enum_to_index(start);
				i < HandyItem::Enum::enum_to_index(end);
				++i
				) {

				if (Flag_.test(HandyItem::Enum::index_to_enum(i))) {
					return false;
				}
			}
		}

		/// <summary>
		/// 指定範囲フラグ [ True ] 判定関数
		/// </summary>
		/// <param name="end">判定終了位置</param>
		/// <param name="start">判定開始位置</param>
		/// <returns>すべて [ True ] なら [ True ]</returns>
		bool RangeAll(T end, T start = T{}) const {
			for (
				auto i = HandyItem::Enum::enum_to_index(start);
				i < HandyItem::Enum::enum_to_index(end);
				++i
				) {

				if (!Flag_.test(HandyItem::Enum::index_to_enum(i))) {
					return false;
				}
			}
		}

	private:
		
		/// <summary>
		/// ビットフラグクラス
		/// </summary>
		HandyItem::bit::BitFlag<T> Flag_{};

	};

	template<HandyItem::Enum::concepts::EnumHasCount T>
	struct BitFlagMenuBase {

		UINT id{};

		LPCWSTR text{};

		T value{};
	};



	template<HandyItem::Enum::concepts::EnumHasCount T>
	class BitFlagMenuExtension : protected BitFlagExtension<T>, public MenuExtension::MenuExtension
	{
	public:

		explicit BitFlagMenuExtension(HWND hWnd, HINSTANCE hInstance) :
			BitFlagExtension<T>{ hWnd,hInstance },
			MenuExtension::MenuExtension{ hWnd ,hInstance },
			HasNoneFlag_{ Concepts::EnumHasNoneCheck<T>() },
			HasNoneFlag_{ Concepts::EnumHasAllCheck<T>() }{};

		~BitFlagMenuExtension() = default;


		bool IlitializeFlagMenu(
			HMENU toSetMenu,
			HMENU rootMenu,
			const wchar_t* menuName,
			std::initializer_list<BitFlagMenuBase<T>> datas
		) {

			std::vector<MENUITEMINFOW> items{};

			for (auto& data : datas) {
				items.push_back(MenuExtension::MenuFactory::MakeMenuItem(
					data.id,
					data.text,
					CheckFlag(data.value)
				));
				if (FlugMenu_.try_emplace(data.id, data.value).second) {
					AddMenuFunction(data.id, [&](WPARAM wParam, LPARAM lParam) {

						ToggleFlag(data.id);
						SetAllFlagMenu(rootMenu);

					});
				}
			}
			return MenuExtension::MenuFactory::AddSubMenu(
				toSetMenu,
				MenuExtension::MenuFactory::CreatePopupMenuToInfo(items),
				menuName
			);
		}

		void ToggleFlag(T value) {
			if (value == T::Count) {
				return;
			}
			else if constexpr (HasNoneFlag_) {
				if (value == T::None) {
					ToggleNone();
					return;
				}
			}
			else if constexpr (HasAllFlag_) {
				if (value == T::All) {
					ToggleAll();
					return;
				}
			}
			else {
				Toggle(value);
			}
		}

		void TpggleFlag(UINT id) {

			const auto it = FlugMenu_.find(id);
			if (it == FlugMenu_.end()) {
				return;
			}

			TpggleFlag(it.second);
		}

		bool CheckNone(T end, T start = T{}) {
			return RangeNone(end, start);
		}
		bool CheckAll(T end, T start = T{}) {
			return RangeAll(end, start);
		}

		void SetMenu(
			HMENU menu,
			UINT id,
			UINT state = CheckFlag(id)
		) {
			MenuExtension::MenuFactory::SetMenuToInfo(
				menu,
				MIIM_STATE,
				state,
				id
			);
		}

		void SetAllFlagMenu(HMENU menu) {

			for (auto& it : FlugMenu_) {
				SetMenu(menu, it.second);
			}
		}

		/// <summary>
		/// フラグチェック関数
		/// </summary>
		/// <param name="flag">ファイル読み込みフラグ</param>
		/// <returns>チェック識別子</returns>
		UINT CheckFlag(T value) {
			return Test(value) ? MFS_CHECKED : MFS_UNCHECKED;
		}

		/// <summary>
		/// フラグチェック関数
		/// </summary>
		/// <param name="id">チェックする値</param>
		/// <returns>チェック識別子</returns>
		UINT CheckFlag(UINT id) {

			const auto it = FlugMenu_.find(id);
			if (it == FlugMenu_.end()) {
				return MFS_UNCHECKED;
			}

			return CheckFlag(it.second);
		}

	private:

		/// <summary>
		/// None識別子確認フラグ
		/// </summary>
		bool HasNoneFlag_{};

		/// <summary>
		/// All識別子確認フラグ
		/// </summary>
		bool HasAllFlag_{};

		/// <summary>
		/// メニューID保存マップ
		/// </summary>
		std::unordered_map<UINT, T> FlugMenu_{};

		/// <summary>
		/// Noneフラグ入力時関数
		/// </summary>
		void ToggleNone() {
			ResetFlag<T>();
			if constexpr (HasNoneFlag_) {
				SetFlag(T::None);
			}
		}

		/// <summary>
		/// Allフラグ入力時フラグ
		/// </summary>
		void ToggleAll() {
			SetFlag<T>();
			if constexpr (HasAllFlag_) {
				SetFlag(T::All);
			}
		}

	};
}