
#include "FmodImplementation.h"
#include <exception>
#include <vector>

namespace Viper
{
	namespace  Audio
	{
		FmodImplementation::FmodImplementation(uint32_t maxChannels)
			: maxChannels(maxChannels), studioSystem(nullptr), fmodSystem(nullptr), nextChannelId(0)
		{
			ErrorCheck(FMOD::Studio::System::create(&studioSystem));
			ErrorCheck(studioSystem->initialize(maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
			ErrorCheck(studioSystem->getLowLevelSystem(&fmodSystem));
		}

		FmodImplementation::~FmodImplementation()
		{
			ErrorCheck(studioSystem->unloadAll());
			ErrorCheck(studioSystem->release());
		}

		void FmodImplementation::Update()
		{
			// find out all the channels which have completed playing
			//std::vector<ChannelMap::iterator> stoppedChannels(channels.size());
			//bool_t isPlaying = false;
			//for (auto it = channels.begin(), itEnd = channels.end(); it != itEnd; ++it)
			//{				
			//	it->second->isPlaying(&isPlaying);
			//	if (!isPlaying)
			//	{
			//		stoppedChannels.emplace_back(it);
			//	}
			//}

			//// remove all channels which have completed playing
			//for(auto& it : stoppedChannels)
			//{
			//	channels.erase(it);
			//}
			ErrorCheck(studioSystem->update());
		}

		void FmodImplementation::ErrorCheck(FMOD_RESULT result)
		{
			if (result != FMOD_OK)
			{
				throw std::runtime_error("FMOD error: result = " + result);
			}
		}

		FMOD_VECTOR FmodImplementation::VectorToFmod(const Vector3& vector)
		{
			FMOD_VECTOR fVec;
			fVec.x = vector.x;
			fVec.y = vector.y;
			fVec.z = vector.z;
			return fVec;
		}
	}
}
