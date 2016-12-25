#ifndef AUDIO_MANAGER_HEADER
#define AUDIO_MANAGER_HEADER


#include <string>
#include <vector>
#include "Types.h"
#include "Vector3.h"

namespace  Viper
{
	namespace Audio
	{
		class FmodImplementation;

		class FmodAudioManager
		{
		private:
			FmodAudioManager();
		public:
			~FmodAudioManager();
			static void CreateInstance();
			static FmodAudioManager* GetInstance();
			static FmodAudioManager* sInstance;

			FmodImplementation* implementation;

			void Init(uint32_t maxChannels);
			void Update();
			void Shutdown();

			void LoadBank(const std::string& bankName, uint32_t flags = 0);
			void LoadEvent(const std::string& eventName);
			void LoadSound(const std::string& soundName, bool_t is3d = true, bool_t isLooping = false, bool_t isStream = false);
			void UnLoadSound(const std::string& soundName);
			
			float32_t GetEventParameter(const std::string& eventName, const std::string& parameterName);
			void SetEventParameter(const std::string& eventName, const std::string& parameterName, float value);
			void SetListener3dAttributes(const Vector3& position, const Vector3& forward, const Vector3& up, const Vector3& velocity = Vector3{0,0,0});
			void SetEvent3dAttributes(const std::string& eventName, const Vector3& position, const Vector3& velocity = Vector3{0,0,0});
			void SetChannel3dAttributes(uint32_t channelId, const Vector3& position, const Vector3& velocity);
			void SetChannelVolume(uint32_t channelId, float volumedB);

			uint32_t PlaySound(const std::string& soundName, const Vector3& position = Vector3{0, 0, 0}, float volumedB = 0.0f);
			void PlayEvent(const std::string& eventName);
			void StopChannel(uint32_t channelId);
			void StopAllChannels();
			void StopEvent(const std::string& eventName, bool_t isImmediate = false);
			
			bool_t IsPlaying(uint32_t channelId) const;
			bool_t IsEventPlaying(const std::string& eventName) const;
			
			float dbToVolume(float dB);
			float VolumeTodB(float volume);
		};
	}
}


#endif	// AUDIO_MANAGER_HEADER
