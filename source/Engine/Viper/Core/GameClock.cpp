#include "Viper.h"
#include "GameClock.h"
#include "GameTime.h"

using namespace std::chrono;

namespace Viper
{
	GameClock::GameClock()
	{
		Reset();
	}

	const high_resolution_clock::time_point& GameClock::StartTime() const
	{
		return mStartTime;
	}

	const high_resolution_clock::time_point& GameClock::CurrentTime() const
	{
		return mCurrentTime;
	}

	const high_resolution_clock::time_point& GameClock::LastTime() const
	{
		return mLastTime;
	}

	void GameClock::Reset()
	{
		mStartTime = high_resolution_clock::now();
		mCurrentTime = mStartTime;
		mLastTime = mCurrentTime;
	}

	void GameClock::UpdateGameTime(GameTime& gameTime)
	{
		mCurrentTime = high_resolution_clock::now();
		gameTime.SetCurrentTime(mCurrentTime);
		gameTime.SetTotalGameTime(duration_cast<milliseconds>(mCurrentTime - mStartTime));
		gameTime.SetElapsedGameTime(duration_cast<milliseconds>(mCurrentTime - mLastTime));
		mLastTime = mCurrentTime;
	}
}
