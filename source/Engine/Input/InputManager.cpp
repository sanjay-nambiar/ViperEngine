#include "Pch.h"
#include "InputManager.h"

using namespace std;

namespace Viper
{
	namespace Input
	{
		InputManager::InputManager() :
			modifierStates(0)
		{
			for (uint32_t index = static_cast<uint32_t>(Button::Key_Unknown);
				index < static_cast<uint32_t>(Button::MaxButtons); ++index)
			{
				buttonStates[static_cast<Button>(index)] = ButtonState::Neutral;
			}
		}

		const unordered_map<Button, ButtonState>& InputManager::GetButtonStates() const
		{
			return buttonStates;
		}

		ButtonState InputManager::GetButtonState(Button button) const
		{
			return buttonStates.at(button);
		}

		uint32_t InputManager::ActiveModifiers() const
		{
			return modifierStates;
		}

		bool InputManager::IsModifierActive(ModifierKey modifier) const
		{
			return ((modifierStates & static_cast<uint32_t>(modifier)) > 0);
		}
	}
}