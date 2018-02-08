#include "Viper.h"
#include "MemoryManager.h"
#include <cassert>

namespace  Viper
{
	namespace Memory
	{
		void* MemoryManager::Allocate(size_t elementSize, uint32_t count)
		{
			assert(count > 0);
			return static_cast<void*>(new uchar8_t*[elementSize * count]);
		}

		void MemoryManager::Free(void* pointer)
		{
			assert(pointer != nullptr);
			delete[] static_cast<uchar8_t*>(pointer);
		}
	}
}