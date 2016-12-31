#ifndef STACK_ALLOCATOR_HEADER
#define STACK_ALLOCATOR_HEADER

#include "Types.h"

namespace Viper
{
	namespace Memory
	{
		typedef uchar8_t* Marker;

		/** A double ended stack memory allocator for implementing memory allocators
		*/
		class StackAllocator
		{
		public:
			/*** Construct a double ended stack allocator with the given total memory
			*  Throws a runtime exception if the allocation fails
			*/
			explicit StackAllocator(uint32_t totalBytes);

			/*** Allocate memory from the stack
			*  A total of (elementSize * count) bytes of memory is allocated from the stack
			*  @param bytes The amount of memory required
			*  @param isTemp Indicates if the memory is temporary in nature i.e. the memory can be released maybe at the end of the frame.
			*  @return A pointer to the base of the allocated memory
			*/
			void* Allocate(size_t bytes, bool isTemp = false);

			/*** Get the marker to the top of the stack
			*  @param isTemp Indicates whether the marker is for the temporary memory or not
			*  @return The marker to the top of the stack
			*/
			Marker GetMarker(bool isTemp = false) const;

			/** Resets the stack top to the given marker
			*  @param marker Marker to which the memory should be deleted
			*/
			void Free(const Marker& marker);

			/*** Clear all the allocations from the stack
			*  Pops all the allocations out of the stack and rolls back the top
			*/
			void Clear();

			/*** Destroy the stack memory
			*  Free any system memory allocated by the class
			*/
			~StackAllocator();
		private:
			Marker memoryBottom;
			Marker memoryTop;
			Marker left;
			Marker right;
		};
	}
}


#endif	// STACK_ALLOCATOR_HEADER
