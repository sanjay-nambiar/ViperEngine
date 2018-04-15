#include "Viper.h"
#include "GameException.h"

using namespace std;

namespace Viper
{
	GameException::GameException(const char* const& message) :
		exception(message)
	{
	}

	GameException::GameException(const std::string& message) :
		exception(message.c_str())
	{
	}
}