#pragma once

#include <exception>

namespace Viper
{
	class GameException : public std::exception
	{
	public:
		GameException(const char* const& message);
	};
}