#include "Viper.h"

namespace Viper
{
	Service::Service(ServiceType type) :
		type(type)
	{
	}

	ServiceType Service::Type() const
	{
		return type;
	}
}
