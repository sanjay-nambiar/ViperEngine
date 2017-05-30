#include "GlfwWindowManager.h"
#include <algorithm>
#include <GLFW/glfw3.h>
#include "GlfwWindowContext.h"

namespace Viper
{
	namespace Window
	{
		void GlfwWindowManager::Initialize()
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		}

		const WindowContext& GlfwWindowManager::CreateGameWindow(std::uint32_t width, std::uint32_t height, const std::string& title)
		{
			GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
			GlfwWindowContext windowContext;
			glfwMakeContextCurrent(window);
			glfwGetFramebufferSize(window, &windowContext.width, &windowContext.height);
			windowContext.window = window;
			windows.emplace_back(windowContext);
			return windows.back();
		}

		bool GlfwWindowManager::BeginUpdate(const WindowContext& windowContext)
		{
			const GlfwWindowContext& context = static_cast<const GlfwWindowContext&>(windowContext);
			if (!glfwWindowShouldClose(context.window))
			{
				glfwPollEvents();
				return true;
			}
			return false;
		}

		void GlfwWindowManager::EndUpdate(const WindowContext& windowContext)
		{
			const GlfwWindowContext& context = static_cast<const GlfwWindowContext&>(windowContext);
			glfwSwapBuffers(context.window);
		}

		void GlfwWindowManager::DestroyGameWindow(const WindowContext& windowContext)
		{
			const GlfwWindowContext& context = static_cast<const GlfwWindowContext&>(windowContext);
			glfwDestroyWindow(context.window);
			windows.erase(std::remove(windows.begin(), windows.end(), context));
		}

		void GlfwWindowManager::Shutdown()
		{
			glfwTerminate();
		}
	}
}
