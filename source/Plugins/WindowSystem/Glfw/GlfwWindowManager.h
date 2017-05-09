#pragma once

#include "Service/WindowManager.h"

struct GLFWwindow;

namespace Viper
{
	namespace Window
	{
		class GlfwWindowManager final: public WindowManager
		{
		public:
			GlfwWindowManager();
			~GlfwWindowManager() = default;

			void Initialize() override;
			std::uint64_t CreateGameWindow(std::uint32_t width, std::uint32_t height, const std::string& title) override;
			bool Update() override;
			void DestroyGameWindow(std::uint64_t windowHandle) override;
			void Shutdown() override;
		private:
			GLFWwindow* window;
		};
	}
}
