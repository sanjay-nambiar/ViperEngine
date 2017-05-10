#include "Pch.h"
#include "InputManager.h"

namespace Viper
{
	InputManager::InputManager() :
		modifierStates(0)
	{
		for (std::uint32_t index = static_cast<std::uint32_t>(Button::Key_Unknown);
			index < static_cast<std::uint32_t>(Button::MaxButtons); ++index)
		{
			buttonStates[static_cast<Button>(index)] = ButtonState::Neutral;
		}
	}

	const std::unordered_map<Button, ButtonState>& InputManager::GetButtonStates() const
	{
		return buttonStates;
	}

	ButtonState InputManager::GetButtonState(Button button) const
	{
		return buttonStates.at(button);
	}

	std::uint32_t InputManager::ActiveModifiers() const
	{
		return modifierStates;
	}

	bool InputManager::IsModifierActive(ModifierKey modifier) const
	{
		return ((modifierStates & static_cast<std::uint32_t>(modifier)) > 0);
	}
}
