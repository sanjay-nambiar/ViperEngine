#ifndef FMOD_AUDIO_MANAGER_HEADER
#define FMOD_AUDIO_MANAGER_HEADER


#include <string>
#include <vector>
#include "service/AudioManager.h"
#include "service/MemoryAllocator.h"
#include "Types.h"
#include "Vector3.h"


namespace FMOD {namespace Studio {
	class Bank;
}
}

namespace  Viper
{
	namespace Audio
	{
		// Forward declarations
		class FmodImplementation;

		class FmodAudioManager : public Viper::AudioManager
		{
			FmodImplementation* implementation;
			MemoryAllocator& allocator;
		public:
			FmodAudioManager(uint32_t maxChannels, MemoryAllocator& allocator);
			~FmodAudioManager();

			void LoadSoundBank(const std::string& bankName, bool_t isAsync = false, bool_t shouldDecompress = false) override;
			void LoadSoundBankEvents(const std::string& bankName) override;
			void UnLoadSoundBank(const std::string& bankName) override;

			void SetListener3dAttributes(const Vector3& position, const Vector3& forward, const Vector3& up,
				const Vector3& velocity = Vector3{0,0,0}) override;
			void SetEvent3dAttributes(const std::string& eventName, const Vector3& position, const Vector3& velocity = Vector3{0,0,0}) override;
			void SetEventGroup(const std::string& eventName, const std::string& groupName) override;
			void SetEventVolume(const std::string& eventName, float volumedB) override;
			void SetGroupVolume(const std::string& groupName, float volumedB) override;

			void PlayEvent(const std::string& eventName) override;
			void StopEvent(const std::string& eventName, bool_t isImmediate = false) override;
			void StopGroup(const std::string& groupName, bool_t isImmediate = false) override;
			void StopAll(bool_t isImmediate = false) override;

			void Update() override;

			bool_t IsPlaying(const std::string& eventName) const override;
		};
	}
}


#endif	// FMOD_AUDIO_MANAGER_HEADER
