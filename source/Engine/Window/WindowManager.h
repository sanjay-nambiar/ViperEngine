#pragma once

#include <cstdint>
#include <string>
#include "Window/WindowContext.h"

namespace Viper
{
	class WindowManager
	{
	public:
		WindowManager() = default;
		virtual ~WindowManager() = default;

		virtual void Initialize() = 0;
		virtual const WindowContext& CreateGameWindow(std::uint32_t width, std::uint32_t height, const std::string& title, bool fullScreen = false) = 0;
		virtual bool BeginUpdate(const WindowContext& windowContext) = 0;
		virtual void EndUpdate(const WindowContext& windowContext) = 0;
		virtual void DestroyGameWindow(const WindowContext& windowContext) = 0;
		virtual void Shutdown() = 0;
	};
}
