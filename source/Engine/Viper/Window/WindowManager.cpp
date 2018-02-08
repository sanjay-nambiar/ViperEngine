#include "Viper.h"

namespace Viper
{
	namespace Window
	{
		const ServiceType WindowManager::Type = ServiceType::WindowManager;

		WindowManager::WindowManager() :
			Service(Type)
		{
		}
	}
}
