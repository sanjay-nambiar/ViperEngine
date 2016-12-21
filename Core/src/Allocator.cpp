#include "Allocator.h"
#include <cassert>

namespace  Viper
{

	void* Allocator::Allocate(size_t elementSize, uint32_t count)
	{
		assert(count > 0);
		return static_cast<void*>(new uchar8_t*[elementSize * count]);
	}

	void Allocator::Free(void* pointer)
	{
		assert(pointer != nullptr);
		delete[] static_cast<uchar8_t*>(pointer);
	}
}
