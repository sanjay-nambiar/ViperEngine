#pragma once

#include "Memory/MemoryAllocator.h"
#include "Core/Types.h"

namespace Viper
{
	namespace Memory
	{
		/** A simple memory manager.
		*  This class is a place holder for implementing a memory manager and it currently uses
		*  regular new and delete to allocate heap memory on demand
		*/
		class MemoryManager : public MemoryAllocator
		{
		public:
			MemoryManager() = default;
			~MemoryManager() = default;

			/** Free allocated memory
			* Free a block of memory previously allocated by the memory manager
			* @param pointer The base pointer to the memory to be freed
			*/
			void Free(void* pointer) override;

		protected:
			void* AllocateInternal(size_t elementSize, uint32_t count) override;
		};
	}
}
