#include "Viper.h"

namespace Viper
{
	namespace Logging
	{
		const ServiceType Logger::Type = ServiceType::Logger;

		Logger::Logger() : Service(Type)
		{
		}
	}
}
