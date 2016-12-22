#ifndef MEMORY_MANAGER_HEADER
#define MEMORY_MANAGER_HEADER

#include "Types.h"

namespace  Viper
{	
	/** A simple memory manager.
	*  This class is a place holder for implementing a memory manager and it currently uses 
	*  regular new and delete to allocate heap memory on demand
	*/
	class MemoryManager
	{
	public:
		MemoryManager() = default;
		~MemoryManager() = default;

		/** Allocate memory
		* A total of (elementSize * count) bytes of memory is allocated
		*/
		void* Allocate(size_t elementSize, uint32_t count);
		/** Free allocated memory
		* Free a block of memory previously allocated by the memory manager
		*/
		void Free(void* pointer);
	};
}


#endif	// MEMORY_MANAGER_HEADER
