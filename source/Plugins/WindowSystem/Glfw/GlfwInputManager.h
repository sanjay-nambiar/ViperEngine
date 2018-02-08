#pragma once

#include "Input/InputManager.h"
#include "GlfwWindowContext.h"
#include <set>

struct GLFWwindow;

namespace Viper
{
	namespace Input
	{
		class GlfwInputManager final: public InputManager
		{
		public:
			GlfwInputManager();
			~GlfwInputManager() = default;

			void Initialize(const WindowContext& windowContext) override;
			void Update() override;
			void Shutdown() override;

		private:
			bool isInitialized;
			Window::GlfwWindowContext context;
			std::set<Button> releasedButtons;

			static void KeyCallback(GLFWwindow*, int, int, int, int);

			static GlfwInputManager* ActiveInputManager;
			static std::unordered_map<std::uint32_t, ModifierKey> ModifierMapping;
			static std::unordered_map<std::uint32_t, Button> KeyMapping;
		};
	}
}
