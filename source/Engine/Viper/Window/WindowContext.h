#pragma once

#include "Core/Types.h"

namespace Viper
{
	namespace Window
	{
		struct WindowContext
		{
			std::int32_t x;
			std::int32_t y;
			std::int32_t width;
			std::int32_t height;

			WindowContext();
			virtual ~WindowContext() = default;

			virtual std::uint64_t WindowHandle() const = 0;
		};
	}
}
