#ifndef MEMORY_MANAGER_HEADER
#define MEMORY_MANAGER_HEADER

#include <cassert>
#include "Types.h"

namespace  Viper
{
	namespace Memory
	{
		class MemoryManager
		{
		public:
			MemoryManager() = default;
			~MemoryManager() = default;

			void* Allocate(size_t elementSize, uint32_t count);
			void Free(void* pointer);
		};
	}
}


#endif	// MEMORY_MANAGER_HEADER
