#ifndef MEMORY_MANAGER_HEADER
#define MEMORY_MANAGER_HEADER

#include "Allocator.h"
#include "Types.h"

namespace  Viper
{	
	class MemoryManager : public Allocator
	{
	public:
		MemoryManager() = default;
		~MemoryManager() = default;
	};
}


#endif	// MEMORY_MANAGER_HEADER
