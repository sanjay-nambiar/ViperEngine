#pragma once

#include "Core/Service.h"
#include "Core/Types.h"

namespace  Viper
{
	namespace Memory
	{
		/** Interface for implementing memory allocators / managers
		*/
		class MemoryAllocator : public Service
		{
		public:
			MemoryAllocator();

			virtual ~MemoryAllocator()
			{};

			/** Allocate memory
			* A total of (sizeof(T) * count) bytes of memory is allocated
			* @param count The number of elements to allocate
			* @return A pointer to the base of the allocated memory
			*/
			template<typename T>
			T* Allocate(uint32_t count = 1);
		
			/** Free allocated memory
			* Free a block of memory previously allocated by the memory manager
			* @param pointer The base pointer to the memory to be freed
			*/
			virtual void Free(void* pointer) = 0;

			static const ServiceType Type;

		protected:
			/** Allocate memory
			* A total of (elementSize * count) bytes of memory is allocated
			* @param elementSize The size of a single element in memory
			* @param count The number of elements to allocate
			* @return A pointer to the base of the allocated memory
			*/
			virtual void* AllocateInternal(size_t elementSize, uint32_t count) = 0;
		};
	}
}

#include "MemoryAllocator.inl"
