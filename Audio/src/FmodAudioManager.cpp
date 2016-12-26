
#include "FmodAudioManager.h"
#include <cassert>
#include "FmodImplementation.h"


#define MAX_EVENT_PATH_LENGTH 100
#define MAX_EVENTS_PER_GROUP 50

namespace Viper
{
	namespace Audio
	{
		FmodAudioManager::FmodAudioManager(uint32_t maxChannels, MemoryAllocator& allocator)
			: implementation(nullptr), allocator(allocator)
		{
			implementation = static_cast<FmodImplementation*>(allocator.Allocate(sizeof(FmodImplementation), 1));
			new (implementation) FmodImplementation(maxChannels);
			if (implementation == nullptr)
			{
				throw std::runtime_error("Out of memory while creating FMOD implementation");
			}
		}

		FmodAudioManager::~FmodAudioManager()
		{
			implementation->~FmodImplementation();
			allocator.Free(implementation);
		}
		
		void FmodAudioManager::Update()
		{
			implementation->Update();
		}

		void FmodAudioManager::LoadSoundBank(const std::string& bankName, bool_t isAsync, bool_t shouldDecompress)
		{
			auto foundIt = implementation->banks.find(bankName);
			assert(foundIt == implementation->banks.end());

			FMOD::Studio::Bank* bank;
			FMOD_STUDIO_LOAD_BANK_FLAGS flags = FMOD_STUDIO_LOAD_BANK_NORMAL;
			if (isAsync)
			{
				flags |= FMOD_STUDIO_LOAD_BANK_NONBLOCKING;
			}
			if (shouldDecompress)
			{
				flags |= FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES;
			}
			FmodImplementation::ErrorCheck(implementation->studioSystem->loadBankFile(bankName.c_str(), flags, &bank));
			implementation->banks[bankName] = bank;
		}

		void FmodAudioManager::LoadSoundBankEvents(const std::string& bankName)
		{
			FMOD::Studio::Bank* bank = implementation->banks[bankName];
			assert(bank != nullptr);

			// Load all events from the bank
			int32_t eventCount;
			FmodImplementation::ErrorCheck(bank->getEventCount(&eventCount));
			assert(eventCount > 0);
			FMOD::Studio::EventDescription** eventDescriptions =
				static_cast<FMOD::Studio::EventDescription**>(allocator.Allocate(sizeof(FMOD::Studio::EventDescription*), eventCount));
			FmodImplementation::ErrorCheck(bank->getEventList(eventDescriptions, eventCount, &eventCount));
			for (int32_t i = 0; i < eventCount; i++)
			{
				char8_t eventPath[MAX_EVENT_PATH_LENGTH];
				int32_t eventPathLength;
				FmodImplementation::ErrorCheck(eventDescriptions[i]->getPath(eventPath, MAX_EVENT_PATH_LENGTH, &eventPathLength));

				// create event instance
				FMOD::Studio::EventInstance* eventInstance = nullptr;
				FmodImplementation::ErrorCheck(eventDescriptions[i]->createInstance(&eventInstance));
				implementation->events[std::string(eventPath)] = eventInstance;
			}
			allocator.Free(eventDescriptions);
		}

		void FmodAudioManager::UnLoadSoundBank(const std::string& soundName)
		{
			auto foundIt = implementation->banks.find(soundName);
			assert(foundIt != implementation->banks.end());

			// Unoad all events from the bank
			int32_t eventCount;
			FmodImplementation::ErrorCheck(foundIt->second->getEventCount(&eventCount));
			assert(eventCount > 0);
			FMOD::Studio::EventDescription** eventDescriptions =
				static_cast<FMOD::Studio::EventDescription**>(allocator.Allocate(sizeof(FMOD::Studio::EventDescription*), eventCount));
			FmodImplementation::ErrorCheck(foundIt->second->getEventList(eventDescriptions, eventCount, &eventCount));
			for (int32_t i = 0; i < eventCount; i++)
			{
				char8_t eventPath[MAX_EVENT_PATH_LENGTH];
				int32_t eventPathLength;
				FmodImplementation::ErrorCheck(eventDescriptions[i]->getPath(eventPath, MAX_EVENT_PATH_LENGTH, &eventPathLength));				
				implementation->events[eventPath]->release();
			}
			allocator.Free(eventDescriptions);

			FmodImplementation::ErrorCheck(foundIt->second->unload());
			implementation->banks.erase(foundIt);
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

		void FmodAudioManager::PlayEvent(const std::string& eventName)
		{
			auto foundit = implementation->events.find(eventName);
			assert(foundit != implementation->events.end());
			foundit->second->start();
		}

		void FmodAudioManager::SetEventGroup(const std::string& eventName, const std::string& groupName)
		{
			if (implementation->eventGroups.find(groupName) == implementation->eventGroups.end())
			{
				implementation->eventGroups[groupName] = std::vector<std::string>(MAX_EVENTS_PER_GROUP);
			}
			implementation->eventGroups[groupName].emplace_back(eventName);
		}

		void FmodAudioManager::SetEventVolume(const std::string& eventName, float volumedB)
		{
			auto foundit = implementation->events.find(eventName);
			assert(foundit != implementation->events.end());
			FmodImplementation::ErrorCheck(foundit->second->setVolume(volumedB));
		}

		void FmodAudioManager::SetGroupVolume(const std::string& groupName, float volumedB)
		{
			if (implementation->eventGroups.find(groupName) == implementation->eventGroups.end())
			{
				return;
			}
			std::vector<std::string>& events = implementation->eventGroups[groupName];
			for(uint32_t i = 0; i < events.size(); i++)
			{
				FmodImplementation::ErrorCheck(implementation->events[events[i]]->setVolume(volumedB));
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

		void FmodAudioManager::StopGroup(const std::string& groupName, bool_t isImmediate)
		{
			if (implementation->eventGroups.find(groupName) == implementation->eventGroups.end())
			{
				return;
			}

			FMOD_STUDIO_STOP_MODE mode;
			mode = isImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
			std::vector<std::string>& events = implementation->eventGroups[groupName];
			for (uint32_t i = 0; i < events.size(); i++)
			{
				implementation->events[events[i]]->stop(mode);
			}
		}

		void FmodAudioManager::StopAll(bool_t isImmediate)
		{
			FMOD_STUDIO_STOP_MODE mode;
			mode = isImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
			for (auto it = implementation->events.begin(); it != implementation->events.end(); ++it)
			{
				it->second->stop(mode);
			}
		}

		bool_t FmodAudioManager::IsPlaying(const std::string& eventName) const
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
	}
}
