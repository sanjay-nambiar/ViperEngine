#include "GlfwInputManager.h"
#include <cassert>
#include <GLFW/glfw3.h>
#include "GlfwWindowContext.h"


namespace Viper
{
	using namespace Window;

	namespace Input
	{
		GlfwInputManager::GlfwInputManager() :
			isInitialized(false)
		{
			context.window = nullptr;
		}

		void GlfwInputManager::Initialize(const WindowContext& windowContext)
		{
			context = static_cast<const GlfwWindowContext&>(windowContext);
			assert(context.window != nullptr);
			ActiveInputManager = this;
			glfwSetKeyCallback(context.window, KeyCallback);
			isInitialized = true;
		}

		void GlfwInputManager::Update()
		{
			assert(isInitialized);
			for (auto& entry : buttonStates)
			{
				if (entry.second == ButtonState::Released && (releasedButtons.find(entry.first) == releasedButtons.end()))
				{
					entry.second = ButtonState::Neutral;
				}
			}
			releasedButtons.clear();
		}

		void GlfwInputManager::Shutdown()
		{
			assert(isInitialized);
			isInitialized = false;
			releasedButtons.clear();
			glfwSetKeyCallback(context.window, nullptr);
		}

		void GlfwInputManager::KeyCallback(GLFWwindow* window, int key, int, int action, int modifiers)
		{
			assert(ActiveInputManager != nullptr);
			assert(ActiveInputManager->isInitialized);
			assert(ActiveInputManager->context.window == window);
			window;
			if (KeyMapping.find(key) == KeyMapping.end())
			{
				return;
			}

			Button button = KeyMapping[static_cast<std::uint32_t>(key)];
			ButtonState& currentState = ActiveInputManager->buttonStates[button];
			
			switch (action)
			{
			case GLFW_PRESS:
				currentState = ButtonState::Pressed;
				break;
			case GLFW_REPEAT:
				currentState = ButtonState::Hold;
				break;
			case GLFW_RELEASE:
				currentState = ButtonState::Released;
				ActiveInputManager->releasedButtons.insert(button);
				break;
			default:
				break;
			}

			ActiveInputManager->modifierStates = 0;
			for (auto& entry : ModifierMapping)
			{
				if ((modifiers & entry.first) > 0)
				{
					ActiveInputManager->modifierStates |= static_cast<std::uint32_t>(entry.second);
				}
			}
		}

		GlfwInputManager* GlfwInputManager::ActiveInputManager = nullptr;

		std::unordered_map<std::uint32_t, ModifierKey> GlfwInputManager::ModifierMapping = {
			{GLFW_MOD_SHIFT, ModifierKey::Shift},
			{GLFW_MOD_CONTROL, ModifierKey::Control},
			{GLFW_MOD_ALT, ModifierKey::Alt},
			{GLFW_MOD_SUPER, ModifierKey::Super}
		};

		std::unordered_map<std::uint32_t, Button> GlfwInputManager::KeyMapping = {
			{GLFW_KEY_UNKNOWN, Button::Key_Unknown},
			{GLFW_KEY_SPACE, Button::Key_Space},
			{GLFW_KEY_APOSTROPHE, Button::Key_Apostrophe},
			{GLFW_KEY_COMMA, Button::Key_Comma},
			{GLFW_KEY_MINUS, Button::Key_Minus},
			{GLFW_KEY_PERIOD, Button::Key_Period},
			{GLFW_KEY_SLASH, Button::Key_Slash},
			{GLFW_KEY_0, Button::Key_0},
			{GLFW_KEY_1, Button::Key_1},
			{GLFW_KEY_2, Button::Key_2},
			{GLFW_KEY_3, Button::Key_3},
			{GLFW_KEY_4, Button::Key_4},
			{GLFW_KEY_5, Button::Key_5},
			{GLFW_KEY_6, Button::Key_6},
			{GLFW_KEY_7, Button::Key_7},
			{GLFW_KEY_8, Button::Key_8},
			{GLFW_KEY_9, Button::Key_9},
			{GLFW_KEY_SEMICOLON, Button::Key_Semicolon},
			{GLFW_KEY_EQUAL, Button::Key_Equal},
			{GLFW_KEY_A, Button::Key_A},
			{GLFW_KEY_B, Button::Key_B},
			{GLFW_KEY_C, Button::Key_C},
			{GLFW_KEY_D, Button::Key_D},
			{GLFW_KEY_E, Button::Key_E},
			{GLFW_KEY_F, Button::Key_F},
			{GLFW_KEY_G, Button::Key_G},
			{GLFW_KEY_H, Button::Key_H},
			{GLFW_KEY_I, Button::Key_I},
			{GLFW_KEY_J, Button::Key_J},
			{GLFW_KEY_K, Button::Key_K},
			{GLFW_KEY_L, Button::Key_L},
			{GLFW_KEY_M, Button::Key_M},
			{GLFW_KEY_N, Button::Key_N},
			{GLFW_KEY_O, Button::Key_O},
			{GLFW_KEY_P, Button::Key_P},
			{GLFW_KEY_Q, Button::Key_Q},
			{GLFW_KEY_R, Button::Key_R},
			{GLFW_KEY_S, Button::Key_S},
			{GLFW_KEY_T, Button::Key_T},
			{GLFW_KEY_U, Button::Key_U},
			{GLFW_KEY_V, Button::Key_V},
			{GLFW_KEY_W, Button::Key_W},
			{GLFW_KEY_X, Button::Key_X},
			{GLFW_KEY_Y, Button::Key_Y},
			{GLFW_KEY_Z, Button::Key_Z},
			{GLFW_KEY_LEFT_BRACKET, Button::Key_LeftBracket},
			{GLFW_KEY_BACKSLASH, Button::Key_Backslash},
			{GLFW_KEY_RIGHT_BRACKET, Button::Key_RightBracket},
			{GLFW_KEY_GRAVE_ACCENT, Button::Key_GraveAccent},
			{GLFW_KEY_WORLD_1, Button::Key_World_1},
			{GLFW_KEY_WORLD_2, Button::Key_World_2},
			{GLFW_KEY_ESCAPE, Button::Key_Escape},
			{GLFW_KEY_ENTER, Button::Key_Enter},
			{GLFW_KEY_TAB, Button::Key_Tab},
			{GLFW_KEY_BACKSPACE, Button::Key_Backspace},
			{GLFW_KEY_INSERT, Button::Key_Insert},
			{GLFW_KEY_DELETE, Button::Key_Delete},
			{GLFW_KEY_RIGHT, Button::Key_Right},
			{GLFW_KEY_LEFT, Button::Key_Left},
			{GLFW_KEY_DOWN, Button::Key_Down},
			{GLFW_KEY_UP, Button::Key_Up},
			{GLFW_KEY_PAGE_UP, Button::Key_PageUp},
			{GLFW_KEY_PAGE_DOWN, Button::Key_PageDown},
			{GLFW_KEY_HOME, Button::Key_Home},
			{GLFW_KEY_END, Button::Key_End},
			{GLFW_KEY_CAPS_LOCK, Button::Key_CapsLock},
			{GLFW_KEY_SCROLL_LOCK, Button::Key_ScrollLock},
			{GLFW_KEY_NUM_LOCK, Button::Key_NumLock},
			{GLFW_KEY_PRINT_SCREEN, Button::Key_PrintScreen},
			{GLFW_KEY_PAUSE, Button::Key_Pause},
			{GLFW_KEY_F1, Button::Key_F1},
			{GLFW_KEY_F2, Button::Key_F2},
			{GLFW_KEY_F3, Button::Key_F3},
			{GLFW_KEY_F4, Button::Key_F4},
			{GLFW_KEY_F5, Button::Key_F5},
			{GLFW_KEY_F6, Button::Key_F6},
			{GLFW_KEY_F7, Button::Key_F7},
			{GLFW_KEY_F8, Button::Key_F8},
			{GLFW_KEY_F9, Button::Key_F9},
			{GLFW_KEY_F10, Button::Key_F10},
			{GLFW_KEY_F11, Button::Key_F11},
			{GLFW_KEY_F12, Button::Key_F12},
			{GLFW_KEY_F13, Button::Key_F13},
			{GLFW_KEY_F14, Button::Key_F14},
			{GLFW_KEY_F15, Button::Key_F15},
			{GLFW_KEY_F16, Button::Key_F16},
			{GLFW_KEY_F17, Button::Key_F17},
			{GLFW_KEY_F18, Button::Key_F18},
			{GLFW_KEY_F19, Button::Key_F19},
			{GLFW_KEY_F20, Button::Key_F20},
			{GLFW_KEY_F21, Button::Key_F21},
			{GLFW_KEY_F22, Button::Key_F22},
			{GLFW_KEY_F23, Button::Key_F23},
			{GLFW_KEY_F24, Button::Key_F24},
			{GLFW_KEY_F25, Button::Key_F25},
			{GLFW_KEY_KP_0, Button::Key_Keypad_0},
			{GLFW_KEY_KP_1, Button::Key_Keypad_1},
			{GLFW_KEY_KP_2, Button::Key_Keypad_2},
			{GLFW_KEY_KP_3, Button::Key_Keypad_3},
			{GLFW_KEY_KP_4, Button::Key_Keypad_4},
			{GLFW_KEY_KP_5, Button::Key_Keypad_5},
			{GLFW_KEY_KP_6, Button::Key_Keypad_6},
			{GLFW_KEY_KP_7, Button::Key_Keypad_7},
			{GLFW_KEY_KP_8, Button::Key_Keypad_8},
			{GLFW_KEY_KP_9, Button::Key_Keypad_9},
			{GLFW_KEY_KP_DECIMAL, Button::Key_Keypad_Decimal},
			{GLFW_KEY_KP_DIVIDE, Button::Key_Keypad_Divide},
			{GLFW_KEY_KP_MULTIPLY, Button::Key_Keypad_Multiply},
			{GLFW_KEY_KP_SUBTRACT, Button::Key_Keypad_Subtract},
			{GLFW_KEY_KP_ADD, Button::Key_Keypad_Add},
			{GLFW_KEY_KP_ENTER, Button::Key_Keypad_Enter},
			{GLFW_KEY_KP_EQUAL, Button::Key_Keypad_Equal},
			{GLFW_KEY_LEFT_SHIFT, Button::Key_LeftShift},
			{GLFW_KEY_LEFT_CONTROL, Button::Key_LeftControl},
			{GLFW_KEY_LEFT_ALT, Button::Key_LeftAlt},
			{GLFW_KEY_LEFT_SUPER, Button::Key_LeftSuper},
			{GLFW_KEY_RIGHT_SHIFT, Button::Key_RightShift},
			{GLFW_KEY_RIGHT_CONTROL, Button::Key_RightControl},
			{GLFW_KEY_RIGHT_ALT, Button::Key_RightAlt},
			{GLFW_KEY_RIGHT_SUPER, Button::Key_RightSuper},
			{GLFW_KEY_MENU, Button::Key_Menu}
		};
	}
}
