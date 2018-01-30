#ifndef FMOD_AUDIO_MANAGER_HEADER
#define FMOD_AUDIO_MANAGER_HEADER


#include <string>
#include "glm/glm.hpp"
#include "Audio/AudioManager.h"
#include "Memory/MemoryAllocator.h"
#include "Core/Types.h"

namespace FMOD
{
	namespace Studio
	{
		class Bank;
	}
}

namespace  Viper
{
	namespace Audio
	{
		// Forward declarations
		class FmodImplementation;

		class FmodAudioManager : public AudioManager
		{
			FmodImplementation* implementation;
			Memory::MemoryAllocator& allocator;
		public:
			FmodAudioManager(uint32_t maxChannels, Memory::MemoryAllocator& allocator);
			~FmodAudioManager();

			void LoadSoundBank(const std::string& bankName, bool_t isAsync = false, bool_t shouldDecompress = false) override;
			void LoadSoundBankEvents(const std::string& bankName) override;
			void UnLoadSoundBank(const std::string& bankName) override;

			void SetListener3dAttributes(const glm::vec3& position, const glm::vec3& forward, const glm::vec3& up,
				const glm::vec3& velocity = glm::vec3{0,0,0}) override;
			void SetEvent3dAttributes(const std::string& eventName, const glm::vec3& position, const glm::vec3& velocity = glm::vec3{0,0,0}) override;
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
