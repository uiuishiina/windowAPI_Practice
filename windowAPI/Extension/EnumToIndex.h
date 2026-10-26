#pragma once
#include<concepts>

/// <summary>
/// 便利アイテム名前空間
/// </summary>
namespace HandyItem {

	/// <summary>
	/// 列挙体名前空間
	/// </summary>
	namespace Enum {

		/// <summary>
		/// テンプレートコンセプト定義用名前空間
		/// </summary>
		namespace concepts {

			/// <summary>
			/// 列挙体指定コンセプト
			/// </summary>
			template<typename T>
			concept IsEnum = std::is_enum_v<T>;

			/// <summary>
			/// 列挙体カウントコンセプト
			/// </summary>
			/// <details>
			/// [ 列挙体 ] かつ [ Countが定義されている ]
			/// 定義されている数を数えるために作成
			/// </details>
			template<typename T>
			concept EnumHasCount = std::is_enum_v <T>
				&& requires { T::Count; };

		}

		/// <summary>
		/// 列挙体変換関数
		/// </summary>
		/// <param name="value">変換する列挙体変数</param>
		/// <returns>変換した値</returns>
		template<concepts::IsEnum T>
		[[nodiscard]] constexpr size_t enum_to_index(
			const T& value
		) {
			return static_cast<size_t>(value);
		}

		template<concepts::IsEnum T>
		[[nodiscard]] constexpr T index_to_enum(
			const size_t& index
		) {
			using U = std::underlying_type_t<T>;

			return static_cast<T>(
				static_cast<U>(T{}) +
				static_cast<U>(index)
				);
		}


	}
}