#include "Viper.h"
#include "Memory/StackAllocator.h"


namespace Viper
{
	namespace Memory
	{
		StackAllocator::StackAllocator(uint32_t totalBytes)
		{
			memoryBottom = static_cast<Marker>(malloc(totalBytes));
			if (memoryBottom == nullptr)
			{
				throw std::runtime_error("Out of memory during StackAllocator initialization");
			}
			memoryTop = memoryBottom + totalBytes;
			left = memoryBottom;
			right = memoryTop;
		}

		void* StackAllocator::Allocate(uint32_t bytes, bool isTemp, uint32_t alignment)
		{
			// make sure the alignment is a power of two
			assert((alignment & (alignment - 1)) == 0);

			// calculate the misalignment in the memory address
			uint32_t misAlignment = static_cast<uint32_t>(reinterpret_cast<uint64_t>(right) & (alignment - 1));

			if (isTemp)
			{
				// get the bumber of bytes to subtract from the memory address to get the alignment 
				uint32_t adjustment = misAlignment;
				adjustment;
				assert((left + bytes + misAlignment) < right);
				right -= bytes;
				return static_cast<void*>(right);
			}
			else
			{
				// get the bumber of bytes to add to the memory address to get the alignment 
				uint32_t adjustment = alignment - misAlignment;
				adjustment;
				assert((left + bytes + adjustment) < right);
				left += bytes;
				return static_cast<void*>(left);
			}
		}

		Marker StackAllocator::GetMarker(bool isTemp) const
		{
			if (isTemp)
			{
				return right;
			}
			else
			{
				return left;
			}
		}

		void StackAllocator::Free(const Marker& marker)
		{
			if (marker > left)
			{
				right = marker;
			}
			else
			{
				left = marker;
			}
		}

		void StackAllocator::Clear()
		{
			left = memoryBottom;
			right = memoryTop;
		}

		StackAllocator::~StackAllocator()
		{
			assert(memoryBottom != nullptr);
			free(memoryBottom);
		}
	}
}
