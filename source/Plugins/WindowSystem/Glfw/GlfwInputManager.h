#pragma once

#include "Service/InputManager.h"

struct GLFWwindow;

namespace Viper
{
	namespace Input
	{
		class GlfwInputManager final: public InputManager
		{
		public:
			GlfwInputManager() = default;
			~GlfwInputManager() = default;

			void Initialize() override;
			void Update() override;
			void Shutdown() override;
		};
	}
}
