#ifndef FMOD_IMPLEMENTATION_HEADER
#define FMOD_IMPLEMENTATION_HEADER

#include <map>
#include <vector>
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "glm/glm.hpp"
#include "Core/Types.h"
#include "FmodAudioManager.h"

namespace Viper
{
	namespace Audio
	{
		class FmodImplementation
		{
		public:
			explicit FmodImplementation(uint32_t maxChannels);
			~FmodImplementation();

			void Update() const;

			FMOD::Studio::System* studioSystem;
			FMOD::System* fmodSystem;

			typedef std::map<std::string, FMOD::Studio::EventInstance*> EventMap;
			typedef std::map<std::string, FMOD::Studio::Bank*> BankMap;
			typedef std::map<std::string, std::vector<std::string>> EventGroupMap;

			BankMap banks;
			EventMap events;
			EventGroupMap eventGroups;

			static void ErrorCheck(FMOD_RESULT result);
			static FMOD_VECTOR VectorToFmod(const glm::vec3& vector);
		};
	}
}


#endif // 
