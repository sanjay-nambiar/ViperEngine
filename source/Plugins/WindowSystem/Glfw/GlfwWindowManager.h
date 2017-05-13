#pragma once

#include "Service/WindowManager.h"
#include "GlfwWindowContext.h"
#include <vector>

struct GLFWwindow;

namespace Viper
{
	namespace Window
	{
		class GlfwWindowManager final: public WindowManager
		{
		public:
			GlfwWindowManager() = default;
			~GlfwWindowManager() = default;

			void Initialize() override;
			const WindowContext& CreateGameWindow(std::uint32_t width, std::uint32_t height, const std::string& title) override;
			bool BeginUpdate(const WindowContext& windowHandle) override;
			void EndUpdate(const WindowContext& windowHandle) override;
			void DestroyGameWindow(const WindowContext& windowHandle) override;
			void Shutdown() override;
		private:
			std::vector<GlfwWindowContext> windows;
		};
	}
}
