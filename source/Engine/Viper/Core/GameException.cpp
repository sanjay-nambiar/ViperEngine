#include "Pch.h"
#include "GameException.h"

using namespace std;

namespace Viper
{
	GameException::GameException(const char* const& message) :
		exception(message)
	{
	}
}