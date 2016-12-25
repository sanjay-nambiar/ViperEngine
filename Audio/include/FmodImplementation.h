#ifndef FMOD_IMPLEMENTATION_HEADER
#define FMOD_IMPLEMENTATION_HEADER

#include <map>
#include <string>
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "Types.h"
#include "Vector3.h"

namespace Viper
{
	namespace Audio
	{
		class FmodImplementation
		{
		public:
			explicit FmodImplementation(uint32_t maxChannels);
			~FmodImplementation();

			void Update();

			const uint32_t maxChannels;
			FMOD::Studio::System* studioSystem;
			FMOD::System* fmodSystem;
			uint32_t nextChannelId;

			typedef std::map<std::string, FMOD::Sound*> SoundMap;
			typedef std::map<uint32_t, FMOD::Channel*> ChannelMap;
			typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
			typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;

			BankMap banks;
			EventMap events;
			SoundMap sounds;
			ChannelMap channels;

			static void ErrorCheck(FMOD_RESULT result);
			static FMOD_VECTOR VectorToFmod(const Vector3& vector);
		};
	}
}


#endif // 
