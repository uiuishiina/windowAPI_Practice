#pragma once

#include"MenuExtension.h"

#include<unordered_map>
#include<optional>
#include<concepts>

namespace ExclusiveMenuExtension {

	template<typename T>
	struct ExclusiveMenuBase {

		UINT id{};

		LPCWSTR text{};

		T value{};
	};

	namespace Concept {

		template<typename Menu,typename T>
		concept IsExclusiveMenu = std::derived_from<Menu, ExclusiveMenuBase<T>>;
	}


	template<typename T, typename Menu>
		requires Concept::IsExclusiveMenu<Menu, T>
	class ExclusiveMenuExtension
	{
	public:

		explicit ExclusiveMenuExtension(
			std::initializer_list<Menu> data,
			T defaultValue = T{}
		) {
			SetMenuData(data);
			CurrentValue_ = defaultValue;
		}

		explicit ExclusiveMenuExtension(
			std::span<const Menu> data,
			T defaultValue = T{}
		) {
			SetMenuData(data);
			CurrentValue_ = defaultValue;
		}

		bool InitializeExclusiveMenu(
			HMENU toSetMenu,
			const wchar_t* menuName,
			MenuExt::MenuExtension* rootMenu,
			std::function<void(WPARAM, LPARAM)> addFunc = [](WPARAM, LPARAM) {}
		) {

			std::vector<MENUITEMINFOW> items{};

			for (const auto& data : MenuData_) {
				items.push_back(MenuExt::MenuFactory::MakeMenuItem(
					data.id,
					data.text,
					CheckCurrentValue(data.value)
				));

				rootMenu->AddMenuFunction(data.id, [this, id = data.id,rootMenu, addFunc]
				(WPARAM wParam, LPARAM lParam) {

						
						if (
							const auto value = GetValue(id);
							value.has_value()
							) {

							CurrentValue_ = value.value();
						}

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

		void SetMenuData(
			std::span<const Menu> data
		) {
			for (auto& value : data) {
				MenuData_.push_back(value);
				MenuMap_.try_emplace(value.id, value.value);
			}
		}

		void SetMenuData(
			std::initializer_list<Menu> data
		) {
			SetMenuData(std::span<const Menu>{data});
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

			for (auto& it : MenuMap_) {
				SetMenu(menu, it.first, CheckCurrentValue(it.second));
			}
		}

		T GetCurrentValue() const noexcept {
			return CurrentValue_;
		}

	private:

		T CurrentValue_{};

		std::vector<Menu> MenuData_{};
		std::unordered_map<UINT, T> MenuMap_{};

		std::optional<T> GetValue(UINT id) const {

			const auto it = MenuMap_.find(id);
			if (it == MenuMap_.end()) {
				return std::nullopt;
			}

			return it->second;
		}

		UINT CheckCurrentValue(T value) const {
			return CurrentValue_ == value ? MFS_CHECKED : MFS_UNCHECKED;
		}

	};
}