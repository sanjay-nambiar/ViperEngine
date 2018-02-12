#pragma once

#include <exception>
#include <string>

namespace Viper
{
	class GameException : public std::exception
	{
	public:
		GameException(const char* const& message);
		GameException(const std::string& message);
	};
}
