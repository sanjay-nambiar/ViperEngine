#include "GlfwWindowManager.h"
#include <GLFW/glfw3.h>

namespace Viper
{
	namespace Window
	{
		GlfwWindowManager::GlfwWindowManager() :
			window(nullptr)
		{
		}

		void GlfwWindowManager::Initialize()
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		}

		std::uint64_t GlfwWindowManager::CreateGameWindow(std::uint32_t width, std::uint32_t height, const std::string& title)
		{
			window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
			glfwMakeContextCurrent(window);
			return reinterpret_cast<std::uint64_t>(window);
		}

		bool GlfwWindowManager::Update()
		{
			if (!glfwWindowShouldClose(window))
			{
				glfwSwapBuffers(window);
				glfwPollEvents();
				return true;
			}
			return false;
		}

		void GlfwWindowManager::DestroyGameWindow(std::uint64_t)
		{
		}

		void GlfwWindowManager::Shutdown()
		{
			glfwTerminate();
		}
	}
}
