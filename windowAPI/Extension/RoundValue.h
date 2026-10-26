#pragma once

namespace RoundValue {

	template<typename T>
	class RoundValue {
	public:

		T Max_{};
		T Min_{};

		T value_{};

		explicit RoundValue(T Max, T Min) :
			Max_{ Max }, Min_{ Min } {}

		RoundValue(const RoundValue<T>& right) {
			value_ = right.value_;
			Round();
		}
		RoundValue& operator = (const RoundValue<T>& right) {
			value_ = right.value_;
			Round();
		}

		T& operator & () {
			return value_;
		}
		const T& operator & () const {
			return value_;
		}

		/* ===== 加算 ===== */

		[[nodiscard]] T operator + (
			const T& right
			) const {

			const auto ans = value_ + right;
			Round(ans);
			return ans;
		}

		RoundValue& operator += (
			const T& right
			) {

			*this = *this + right;
			return *this;
		}

		/* ===== 減算 ===== */

		[[nodiscard]] T operator - (
			const T& right
			) const {

			const auto ans = value_ - right;
			Round(ans);
			return ans;
		}

		RoundValue& operator -= (
			const T& right
			) {

			*this = *this - right;
			return *this;
		}

		/* ===== 乗算 ===== */

		[[nodiscard]] T operator * (
			const T& right
			) const {

			const auto ans = value_ * right;
			Round(ans);
			return ans;
		}

		RoundValue& operator *= (
			const T& right
			) {

			*this = *this * right;
			return *this;
		}

		/* ===== 除算 ===== */

		[[nodiscard]] T operator / (
			const T& right
			) const {

			const auto ans = value_ / right;
			Round(ans);
			return ans;
		}

		RoundValue& operator /= (
			const T& right
			) {

			*this = *this / right;
			return *this;
		}

		private:

			void Round() {
				if (value_ > Max_) {
					value_ = Max_;
				}
				else if (value_ < Min_) {
					value_ = Min_;
				}
			}
			void Round(T& value) {
				if (value > Max_) {
					value = Max_;
				}
				else if (value < Min_) {
					value = Min_;
				}
			}

	};
}