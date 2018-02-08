#include "Viper.h"

namespace  Viper
{
	namespace Memory
	{
		const ServiceType MemoryAllocator::Type = ServiceType::MemoryAllocator;

		MemoryAllocator::MemoryAllocator() : Service(Type)
		{
		}
	}
}
