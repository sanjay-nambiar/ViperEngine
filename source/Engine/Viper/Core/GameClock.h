#pragma once

#include <chrono>

namespace Viper
{
	class GameTime;

	class GameClock final
	{
	public:
		GameClock();

		const std::chrono::high_resolution_clock::time_point& StartTime() const;
		const std::chrono::high_resolution_clock::time_point& CurrentTime() const;
		const std::chrono::high_resolution_clock::time_point& LastTime() const;

		void Reset();
		void UpdateGameTime(GameTime& gameTime);

	private:
		std::chrono::high_resolution_clock::time_point mStartTime;
		std::chrono::high_resolution_clock::time_point mCurrentTime;
		std::chrono::high_resolution_clock::time_point mLastTime;
	};
}