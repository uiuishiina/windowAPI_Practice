#pragma once
#include"EnumToIndex.h"
#include<bitset>

/// <summary>
/// 便利アイテム名前空間
/// </summary>
namespace HandyItem {

	/// <summary>
	/// ビット名前空間
	/// </summary>
	namespace bit {

		/* ========== ビットフラグクラス定義 ========== */

		/// <summary>
		/// ビットフラグクラス
		/// </summary>
		/// <typeparam name="T">Bitフラグに使う列挙体の型</typeparam>
		template<Enum::concepts::EnumHasCount T>
		class BitFlag
		{
		public:
			/* ===== メンバー関数 ===== */

			/// <summary>
			/// コンストラクタ
			/// </summary>
			BitFlag() = default;

			template<Enum::concepts::EnumHasCount...Args>
			BitFlag(Args... args) {
				(bit_.set(Enum::enum_to_index(args)), ...);
			}

			/// <summary>
			/// デストラクタ
			/// </summary>
			~BitFlag() = default;

			/// <summary>
			/// ビットセット関数
			/// </summary>
			/// <param name="value">対応するenum</param>
			void set(
				T value
			) {
				bit_.set(Enum::enum_to_index(value));
			}

			/// <summary>
			/// ビットリセット関数
			/// </summary>
			/// <param name="value">対応するenum</param>
			void reset(
				T value
			) {
				bit_.reset(Enum::enum_to_index(value));
			}

			/// <summary>
			/// ビットテスト関数
			/// </summary>
			/// <param name="value">対応するenum</param>
			/// <returns>テスト結果</returns>
			[[nodiscard]] bool test(
				T value
			) const {
				return bit_.test(Enum::enum_to_index(value));
			}

			/// <summary>
			/// [ true ] ビット存在判定関数
			/// </summary>
			/// <returns> 一つでもビットが立っていたら [ true ] </returns>
			[[nodiscard]] bool any() const {
				return bit_.any();
			}

			/// <summary>
			/// 全ビット [ true ] 判定関数
			/// </summary>
			void all() {
				return bit_.all();
			}

			/// <summary>
			/// 全ビット [ false ] 判定関数
			/// </summary>
			/// <returns> すべてのビットが立っていないなら [ true ] </returns>
			[[nodiscard]] bool none() const {
				return bit_.none();
			}

			/// <summary>
			/// ビットトグル関数
			/// </summary>
			/// <param name="value"></param>
			void toggle(T value) {

				test(value) ? reset(value) : set(value);
			}

			/// <summary>
			/// 全ビット [ true ] 関数
			/// </summary>
			void set_all() {
				bit_.set();
			}

			/// <summary>
			/// 全ビット [ false ] 関数
			/// </summary>
			void set_none() {
				bit_.reset();
			}

		private:
			/* ===== メンバー変数 ===== */

			/// <summary>
			/// ビットフラグ
			/// </summary>
			std::bitset<Enum::enum_to_index(T::Count)> bit_;

		};
	}
}