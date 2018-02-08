#include "Viper.h"

namespace Viper
{
	namespace Audio
	{
		const ServiceType AudioManager::Type = ServiceType::AudioManager;

		AudioManager::AudioManager() :
			Service(Type)
		{
		}
	}
}
