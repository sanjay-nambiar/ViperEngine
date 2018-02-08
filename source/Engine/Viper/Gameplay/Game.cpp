#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Gameplay
	{
		Game::Game(uint32_t width, uint32_t height, const string& title) :
			width(width), height(height), title(title)
		{
		}

		uint32_t Game::Width() const
		{
			return width;
		}

		uint32_t Game::Height() const
		{
			return height;
		}

		const string& Game::Title() const
		{
			return title;
		}
	}
}
