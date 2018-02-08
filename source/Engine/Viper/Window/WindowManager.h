#pragma once

#include <string>
#include "Core/Service.h"
#include "Core/Types.h"
#include "Window/WindowContext.h"

namespace Viper
{
	namespace Window
	{
		class WindowManager : public Service
		{
		public:
			WindowManager();
			virtual ~WindowManager() = default;

			virtual void Initialize() = 0;
			virtual const WindowContext& CreateGameWindow(std::uint32_t width, std::uint32_t height, const std::string& title, bool fullScreen = false) = 0;
			virtual bool BeginUpdate(const WindowContext& windowContext) = 0;
			virtual void EndUpdate(const WindowContext& windowContext) = 0;
			virtual void DestroyGameWindow(const WindowContext& windowContext) = 0;
			virtual void Shutdown() = 0;

			static const ServiceType Type;
		};
	}
}
