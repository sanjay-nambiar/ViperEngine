#include "memory/StackAllocator.h"
#include <cassert>
#include <stdexcept>

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

		void* StackAllocator::Allocate(size_t bytes, bool isTemp)
		{
			assert((left + bytes) < right);
			if (isTemp)
			{
				right -= bytes;
				return static_cast<void*>(right);
			}
			else
			{
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
