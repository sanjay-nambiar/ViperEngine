#ifndef FMOD_IMPLEMENTATION_HEADER
#define FMOD_IMPLEMENTATION_HEADER

#include <map>
#include <string>
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "Types.h"

namespace Viper
{
	namespace Audio
	{
		class FmodImplementation
		{
		public:
			FmodImplementation();
			~FmodImplementation();

			void Update();

		private:
			FMOD::Studio::System* mpStudioSystem;
			FMOD::System* mpSystem;

			uint32_t mnNextChannelId;

			typedef std::map<std::string, FMOD::Sound*> SoundMap;
			typedef std::map<uint32_t, FMOD::Channel*> ChannelMap;
			typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
			typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;

			BankMap mBanks;
			EventMap mEvents;
			SoundMap mSounds;
			ChannelMap mChannels;
		};
	}
}


#endif // 
