
#include "FmodAudioManager.h"
#include <cassert>
#include <cmath>
#include <exception>
#include "FmodImplementation.h"

namespace Viper
{
	namespace Audio
	{
		FmodAudioManager* FmodAudioManager::sInstance = nullptr;

		FmodAudioManager::FmodAudioManager() : implementation(nullptr)
		{
		}

		FmodAudioManager::~FmodAudioManager()
		{
			assert(sInstance != nullptr);
			delete sInstance;
		}

		void FmodAudioManager::CreateInstance()
		{
			assert(sInstance == nullptr);
			sInstance = new FmodAudioManager();
		}

		FmodAudioManager* FmodAudioManager::GetInstance()
		{
			assert(sInstance != nullptr);
			return sInstance;
		}

		void FmodAudioManager::Init(uint32_t maxChannels)
		{
			implementation = new FmodImplementation(maxChannels);
			if (implementation == nullptr)
			{
				throw std::runtime_error("Out of memory while creating FMOD implementation");
			}
		}
		
		void FmodAudioManager::Update()
		{
			implementation->Update();
		}

		void FmodAudioManager::Shutdown()
		{
			assert(implementation != nullptr);
			delete implementation;
		}

		void FmodAudioManager::LoadSound(const std::string& soundName, bool_t is3d, bool_t isLooping, bool_t isStream)
		{
			auto foundIt = implementation->sounds.find(soundName);
			assert(foundIt == implementation->sounds.end());

			FMOD_MODE mode = FMOD_DEFAULT;
			mode |= is3d ? FMOD_3D : FMOD_2D;
			mode |= isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
			mode |= isStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
			FMOD::Sound* sound = nullptr;
			FmodImplementation::ErrorCheck(implementation->fmodSystem->createSound(soundName.c_str(), mode, nullptr, &sound));
			implementation->sounds[soundName] = sound;
		}

		void FmodAudioManager::LoadBank(const std::string& bankName, uint32_t flags)
		{
			auto foundIt = implementation->banks.find(bankName);
			assert(foundIt == implementation->banks.end());

			FMOD::Studio::Bank* bank;
			FmodImplementation::ErrorCheck(implementation->studioSystem->loadBankFile(bankName.c_str(), flags, &bank));
			implementation->banks[bankName] = bank;
		}

		void FmodAudioManager::LoadEvent(const std::string& eventName)
		{
			auto foundIt = implementation->events.find(eventName);
			assert(foundIt == implementation->events.end());

			FMOD::Studio::EventDescription* eventDescription = nullptr;
			FmodImplementation::ErrorCheck(implementation->studioSystem->getEvent(eventName.c_str(), &eventDescription));

			FMOD::Studio::EventInstance* eventInstance = nullptr;
			FmodImplementation::ErrorCheck(eventDescription->createInstance(&eventInstance));
			implementation->events[eventName] = eventInstance;
		}

		void FmodAudioManager::UnLoadSound(const std::string& soundName)
		{
			auto foundIt = implementation->sounds.find(soundName);
			assert(foundIt != implementation->sounds.end());				
			FmodImplementation::ErrorCheck(foundIt->second->release());
			implementation->sounds.erase(foundIt);
		}

		float32_t FmodAudioManager::GetEventParameter(const std::string& eventName, const std::string& parameterName)
		{
			auto foundIt = implementation->events.find(eventName);
			assert(foundIt != implementation->events.end());

			FMOD::Studio::ParameterInstance* parameter = nullptr;
			FmodImplementation::ErrorCheck(foundIt->second->getParameter(parameterName.c_str(), &parameter));
			float32_t parameterValue;
			FmodImplementation::ErrorCheck(parameter->getValue(&parameterValue));
			return parameterValue;
		}

		void FmodAudioManager::SetEventParameter(const std::string& eventName, const std::string& parameterName, float value)
		{
			auto foundIt = implementation->events.find(eventName);
			assert(foundIt != implementation->events.end());

			FMOD::Studio::ParameterInstance* parameter = nullptr;
			FmodImplementation::ErrorCheck(foundIt->second->getParameter(parameterName.c_str(), &parameter));
			FmodImplementation::ErrorCheck(parameter->setValue(value));
		}

		void FmodAudioManager::SetListener3dAttributes(const Vector3& position, const Vector3& forward, const Vector3& up, const Vector3& velocity)
		{
			FMOD_VECTOR fmodPosition = FmodImplementation::VectorToFmod(position);
			FMOD_VECTOR fmodVelocity = FmodImplementation::VectorToFmod(velocity);
			FMOD_VECTOR fmodForward = FmodImplementation::VectorToFmod(forward);
			FMOD_VECTOR fmodUp = FmodImplementation::VectorToFmod(up);
			FmodImplementation::ErrorCheck(implementation->fmodSystem->set3DListenerAttributes(0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp));
		}

		void FmodAudioManager::SetEvent3dAttributes(const std::string& eventName, const Vector3& position, const Vector3& velocity)
		{
			auto foundIt = implementation->events.find(eventName);
			assert(foundIt != implementation->events.end());
			
			FMOD_3D_ATTRIBUTES attributes = {{0}};
			attributes.position = FmodImplementation::VectorToFmod(position);
			attributes.velocity = FmodImplementation::VectorToFmod(velocity);
			foundIt->second->set3DAttributes(&attributes);
		}

		void FmodAudioManager::SetChannel3dAttributes(uint32_t channelId, const Vector3& position, const Vector3& velocity)
		{
			auto foundIt = implementation->channels.find(channelId);
			assert(foundIt != implementation->channels.end());
			FMOD_VECTOR fmodPosition = FmodImplementation::VectorToFmod(position);
			FMOD_VECTOR fmodVelocity = FmodImplementation::VectorToFmod(velocity);
			FmodImplementation::ErrorCheck(foundIt->second->set3DAttributes(&fmodPosition, &fmodVelocity));
		}

		void FmodAudioManager::SetChannelVolume(uint32_t channelId, float volumedB)
		{
			auto foundIt = implementation->channels.find(channelId);
			assert(foundIt != implementation->channels.end());
			FmodImplementation::ErrorCheck(foundIt->second->setVolume(dbToVolume(volumedB)));
		}

		uint32_t FmodAudioManager::PlaySound(const std::string& soundName, const Vector3& position, float volumedB)
		{
			uint32_t channelId = implementation->nextChannelId++;
			auto foundIt = implementation->sounds.find(soundName);
			assert(foundIt != implementation->sounds.end());
			
			FMOD::Channel* channel = nullptr;
			FmodImplementation::ErrorCheck(implementation->fmodSystem->playSound(foundIt->second, nullptr, true, &channel));
			FMOD_MODE currentMode;
			foundIt->second->getMode(&currentMode);
			if (currentMode & FMOD_3D)
			{
				FMOD_VECTOR positionFmod = FmodImplementation::VectorToFmod(position);
				FmodImplementation::ErrorCheck(channel->set3DAttributes(&positionFmod, nullptr));
			}
			FmodImplementation::ErrorCheck(channel->setVolume(dbToVolume(volumedB)));
			FmodImplementation::ErrorCheck(channel->setPaused(false));
			implementation->channels[channelId] = channel;
			return channelId;
		}

		void FmodAudioManager::PlayEvent(const std::string& eventName)
		{
			auto foundit = implementation->events.find(eventName);
			assert(foundit != implementation->events.end());
			foundit->second->start();
		}

		void FmodAudioManager::StopChannel(uint32_t channelId)
		{
			auto foundIt = implementation->channels.find(channelId);
			assert(foundIt != implementation->channels.end());
			FmodImplementation::ErrorCheck(foundIt->second->stop());
		}
		
		void FmodAudioManager::StopAllChannels()
		{
			for (auto it = implementation->channels.begin(), itEnd = implementation->channels.end(); it != itEnd; ++it)
			{
				FmodImplementation::ErrorCheck(it->second->stop());
			}
		}

		void FmodAudioManager::StopEvent(const std::string& eventName, bool_t isImmediate)
		{
			auto foundIt = implementation->events.find(eventName);
			assert(foundIt != implementation->events.end());

			FMOD_STUDIO_STOP_MODE mode;
			mode = isImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
			FmodImplementation::ErrorCheck(foundIt->second->stop(mode));
		}

		bool_t FmodAudioManager::IsPlaying(uint32_t channelId) const
		{
			auto foundIt = implementation->channels.find(channelId);
			if(foundIt == implementation->channels.end())
			{
				return false;
			}

			bool_t isPlaying = false;
			foundIt->second->isPlaying(&isPlaying);
			return isPlaying;
		}

		bool_t FmodAudioManager::IsEventPlaying(const std::string& eventName) const
		{
			auto foundIt = implementation->events.find(eventName);
			if (foundIt == implementation->events.end())
			{
				return false;
			}				

			FMOD_STUDIO_PLAYBACK_STATE* state = nullptr;
			if (foundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING)
			{
				return true;
			}
			return false;
		}

		float FmodAudioManager::dbToVolume(float dB)
		{
			return powf(10.0f, 0.05f * dB);
		}

		float FmodAudioManager::VolumeTodB(float volume)
		{
			return 20.0f * log10f(volume);
		}		
	}
}
