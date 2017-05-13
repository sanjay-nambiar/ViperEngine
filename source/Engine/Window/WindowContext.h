#pragma once

#include <cstdint>

namespace Viper
{
	struct WindowContext
	{
		std::int32_t x;
		std::int32_t y;
		std::int32_t width;
		std::int32_t height;

		WindowContext();
		virtual ~WindowContext() = default;
	};
}
