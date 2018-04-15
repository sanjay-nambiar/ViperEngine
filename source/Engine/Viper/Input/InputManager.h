#pragma once

#include <unordered_map>
#include "Core/Service.h"
#include "InputEnums.h"
#include "Window/WindowContext.h"

namespace Viper
{
	namespace Input
	{
		class InputManager : public Service
		{
		public:
			InputManager();
			virtual ~InputManager() = default;

			virtual void Initialize(const Window::WindowContext& windowContext) = 0;
			virtual void Update() = 0;
			virtual void Shutdown() = 0;

			const std::unordered_map<Button, ButtonState>& GetButtonStates() const;
			ButtonState GetButtonState(Button) const;
			std::uint32_t ActiveModifiers() const;
			bool IsModifierActive(ModifierKey modifier) const;

			static const ServiceType Type;
		protected:
			std::unordered_map<Button, ButtonState> buttonStates;
			std::uint32_t modifierStates;
		};
	}
}
