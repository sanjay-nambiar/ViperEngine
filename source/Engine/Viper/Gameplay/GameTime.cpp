#include "Viper.h"
#include "GameTime.h"

using namespace std::chrono;

namespace Viper
{
	namespace Gameplay
	{
		GameTime::GameTime() :
			mTotalGameTime(0), mElapsedGameTime(0)
		{
		}

		const high_resolution_clock::time_point& GameTime::CurrentTime() const
		{
			return mCurrentTime;
		}

		void GameTime::SetCurrentTime(const high_resolution_clock::time_point& currentTime)
		{
			mCurrentTime = currentTime;
		}

		const milliseconds& GameTime::TotalGameTime() const
		{
			return mTotalGameTime;
		}

		void GameTime::SetTotalGameTime(const milliseconds& totalGameTime)
		{
			mTotalGameTime = totalGameTime;
		}

		const milliseconds& GameTime::ElapsedGameTime() const
		{
			return mElapsedGameTime;
		}

		void GameTime::SetElapsedGameTime(const milliseconds& elapsedGameTime)
		{
			mElapsedGameTime = elapsedGameTime;
		}

		duration<float32_t> GameTime::TotalGameTimeSeconds() const
		{
			return duration_cast<duration<float32_t>>(mTotalGameTime);
		}

		duration<float32_t> GameTime::ElapsedGameTimeSeconds() const
		{
			return duration_cast<duration<float32_t>>(mElapsedGameTime);
		}
	}
}
