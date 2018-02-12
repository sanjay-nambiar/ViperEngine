#pragma once

#include "GLFW/glfw3.h"
#include "Window/WindowContext.h"

namespace Viper
{
	namespace Window
	{
		struct GlfwWindowContext final : WindowContext
		{
			GLFWwindow* window;

			bool operator==(const GlfwWindowContext& rhs) const;
			bool operator!=(const GlfwWindowContext& rhs) const;
		};
	}
}
