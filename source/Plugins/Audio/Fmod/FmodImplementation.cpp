
#include "FmodImplementation.h"
#include <exception>
#include <vector>
#include "fmod_studio.hpp"
#include "fmod_studio_common.h"

using namespace FMOD;
using namespace glm;

namespace Viper
{
	namespace  Audio
	{
		FmodImplementation::FmodImplementation(uint32_t maxChannels)
			: studioSystem(nullptr), fmodSystem(nullptr), banks(BankMap()),
			events(EventMap()), eventGroups(EventGroupMap())
		{
			ErrorCheck(Studio::System::create(&studioSystem));
			ErrorCheck(studioSystem->initialize(maxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
			ErrorCheck(studioSystem->getLowLevelSystem(&fmodSystem));
		}

		FmodImplementation::~FmodImplementation()
		{
			ErrorCheck(studioSystem->unloadAll());
			ErrorCheck(studioSystem->release());
		}

		void FmodImplementation::Update() const
		{
			ErrorCheck(studioSystem->update());
		}

		void FmodImplementation::ErrorCheck(FMOD_RESULT result)
		{
			if (result != FMOD_OK)
			{
				throw std::runtime_error("FMOD error: result = " + result);
			}
		}

		FMOD_VECTOR FmodImplementation::VectorToFmod(const vec3& vector)
		{
			FMOD_VECTOR fVec;
			fVec.x = vector.x;
			fVec.y = vector.y;
			fVec.z = vector.z;
			return fVec;
		}
	}
}
