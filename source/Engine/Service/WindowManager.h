#pragma once

#include <cstdint>
#include <string>

namespace Viper
{
	class WindowManager
	{
	public:
		WindowManager() = default;
		virtual ~WindowManager() = default;

		virtual void Initialize() = 0;
		virtual std::uint64_t CreateGameWindow(std::uint32_t width, std::uint32_t height, const std::string& title) = 0;
		virtual bool BeginUpdate() = 0;
		virtual void EndUpdate() = 0;
		virtual void DestroyGameWindow(std::uint64_t windowHandle) = 0;
		virtual void Shutdown() = 0;
	};
}
