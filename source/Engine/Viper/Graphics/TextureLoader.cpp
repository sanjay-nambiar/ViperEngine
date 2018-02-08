#include "Viper.h"

namespace Viper
{
	namespace Graphics
	{
		const ServiceType TextureLoader::Type = ServiceType::TextureLoader;

		TextureLoader::TextureLoader() :
			Service(Type)
		{
		}
	}
}
