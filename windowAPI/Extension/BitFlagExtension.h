#pragma once

#include"../ApplocationExtensionBase.h"
#include"../Extension/MenuExtension.h"

#include"BitFlag.h"

#include<unordered_map>

namespace BitFlagExtension {

	namespace Concepts {

		template<typename T>
		concept EnumHasNone = std::is_enum_v <T>
			&& requires { T::none; };

		template<typename T>
		concept EnumHasAll = std::is_enum_v <T>
			&& requires { T::all; };

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

		template<typename T, typename U>
		concept IsSame = std::same_as<T, U>;
	}

	/// <summary>
	/// ビットフラグ機能拡張
	/// </summary>
	/// <typeparam name="T">ビットフラグ化する列挙体</typeparam>
	template<HandyItem::Enum::concepts::EnumHasCount T>
	class BitFlagExtension
	{
	public:

		template<Concepts::IsSame<T>... Args>
		explicit BitFlagExtension(Args... args) :
			Flag_{ args... } {}

		/// <summary>
		/// デストラクタ
		/// </summary>
		~BitFlagExtension() = default;

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

				if (Flag_.test(HandyItem::Enum::index_to_enum<T>(i))) {
					return false;
				}
			}
			return true;
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

				if (!Flag_.test(HandyItem::Enum::index_to_enum<T>(i))) {
					return false;
				}
			}
			return true;
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
	class BitFlagMenuExtension : public BitFlagExtension<T>
	{
	public:

		template<Concepts::IsSame<T>... Args>
		explicit BitFlagMenuExtension(Args... args) :
			BitFlagExtension<T>{}{
			((this->ToggleFlag(args)), ...);
		};

		~BitFlagMenuExtension() = default;

		template<typename MenuData>
		requires std::derived_from<MenuData,BitFlagMenuBase<T>>
		bool IlitializeFlagMenu(
			HMENU toSetMenu,
			const wchar_t* menuName,
			std::span<const MenuData> datas,
			MenuExt::MenuExtension* rootMenu,
			std::function<void(WPARAM, LPARAM)> addFunc = [](WPARAM, LPARAM){}
		) {

			std::vector<MENUITEMINFOW> items{};

			for (auto& data : datas) {
				items.push_back(MenuExt::MenuFactory::MakeMenuItem(
					data.id,
					data.text,
					CheckFlag(data.value)
				));

				FlugMenuMap_.try_emplace(data.id, data.value);

				rootMenu->AddMenuFunction(data.id, [this, id = data.id, value = data.value, rootMenu, addFunc]
				(WPARAM wParam, LPARAM lParam) {

						ToggleFlag(id);

						CheakNoneAll();

						ReloadAllFlagMenu(rootMenu->WindowMenu_);
						addFunc(wParam, lParam);
					});
			}
			return MenuExt::MenuFactory::AddSubMenu(
				toSetMenu,
				MenuExt::MenuFactory::CreatePopupMenuToInfo(items),
				menuName
			);
		}

		void SetNoneRange(T end, T start = T{}) {
			NoneRange_ = { end,start };
		}

		void SetAllRange(T end, T start = T{}) {
			AllRange_ = { end,start };
		}

		void ToggleFlag(T value) {
			if (value == T::Count) {
				return;
			}
			
			if constexpr (HasNoneFlag_) {
				if (value == T::None) {
					ToggleNone();
					return;
				}
			}

			if constexpr (HasAllFlag_) {
				if (value == T::All) {
					ToggleAll();
					return;
				}
			}
			this->Toggle(value);
		}

		void ToggleFlag(const UINT& id) {

			const auto it = FlugMenuMap_.find(id);
			if (it == FlugMenuMap_.end()) {
				return;
			}

			ToggleFlag(it->second);
		}

		bool CheckNone() {
			return this->RangeNone(NoneRange_.first, NoneRange_.second);
		}
		bool CheckAll() {
			return this->RangeAll(AllRange_.first, AllRange_.second);
		}

		void SetMenu(
			HMENU menu,
			UINT id,
			UINT state
		) {
			MenuExt::MenuFactory::SetMenuToInfo(
				menu,
				MIIM_STATE,
				state,
				id
			);
		}

		void ReloadAllFlagMenu(HMENU menu) {

			CheakNoneAll();

			for (auto& it : FlugMenuMap_) {
				SetMenu(menu, it.first, CheckFlag(it.first));
			}
		}

		/// <summary>
		/// フラグチェック関数
		/// </summary>
		/// <param name="value">ファイル読み込みフラグ</param>
		/// <returns>チェック識別子</returns>
		UINT CheckFlag(T value) {
			return this->Test(value) ? MFS_CHECKED : MFS_UNCHECKED;
		}

		/// <summary>
		/// フラグチェック関数
		/// </summary>
		/// <param name="id">チェックする値</param>
		/// <returns>チェック識別子</returns>
		UINT CheckFlag(UINT id) {

			const auto it = FlugMenuMap_.find(id);
			if (it == FlugMenuMap_.end()) {
				return MFS_UNCHECKED;
			}

			return CheckFlag(it->second);
		}

	private:

		/// <summary>
		/// None識別子確認フラグ
		/// </summary>
		static constexpr bool HasNoneFlag_ =
			Concepts::EnumHasNoneCheck<T>();

		/// <summary>
		/// All識別子確認フラグ
		/// </summary>
		static constexpr bool HasAllFlag_ =
			Concepts::EnumHasAllCheck<T>();

		std::pair<T, T> NoneRange_{};
		std::pair<T, T> AllRange_{};

		/// <summary>
		/// メニューID保存マップ
		/// </summary>
		std::unordered_map<UINT, T> FlugMenuMap_{};

		/// <summary>
		/// Noneフラグ入力時関数
		/// </summary>
		void ToggleNone() {
			this->ResetFlag();
			if constexpr (HasNoneFlag_) {
				this->SetFlag(T::None);
			}
		}

		/// <summary>
		/// Allフラグ入力時フラグ
		/// </summary>
		void ToggleAll() {
			this->SetFlag();
			if constexpr (HasNoneFlag_) {
				this->ResetFlag(T::None);
			}
		}

		void CheakNoneAll() {

			if (CheckNone()) {
				if constexpr (HasNoneFlag_) {
					this->SetFlag(T::None);
				}
			}
			else {
				if constexpr (HasNoneFlag_) {
					this->ResetFlag(T::None);
				}
			}

			if (CheckAll()) {
				if constexpr (HasAllFlag_) {
					this->SetFlag(T::All);
				}
			}
			else {
				if constexpr (HasAllFlag_) {
					this->ResetFlag(T::All);
				}
			}
		}

	};
}