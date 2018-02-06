#pragma once

#include "Core/Types.h"

namespace  Viper
{
	namespace Memory
	{
		/** Interface for implementing memory allocators / managers
		*/
		class MemoryAllocator
		{
		public:
			virtual ~MemoryAllocator()
			{};

			/** Allocate memory
			* A total of (elementSize * count) bytes of memory is allocated
			* @param elementSize The size of a single element in memory
			* @param count The number of elements to allocate
			* @return A pointer to the base of the allocated memory
			*/
			virtual void* Allocate(size_t elementSize, uint32_t count) = 0;
		
			/** Free allocated memory
			* Free a block of memory previously allocated by the memory manager
			* @param pointer The base pointer to the memory to be freed
			*/
			virtual void Free(void* pointer) = 0;
		};
	}
}