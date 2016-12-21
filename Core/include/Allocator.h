#ifndef ALLOCATOR_INTERFACE_HEADER
#define ALLOCATOR_INTERFACE_HEADER

#include "Types.h"

namespace  Viper
{
	class Allocator
	{
	public:
		void* Allocate(size_t elementSize, uint32_t count);
		void Free(void* pointer);
	};
}


#endif	// ALLOCATOR_INTERFACE_HEADER
