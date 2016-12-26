#ifndef VIPER_QUEUE_HEADER
#define VIPER_QUEUE_HEADER

#include <cassert>
#include "service/MemoryAllocator.h"
#include "Types.h"


namespace Viper
{
	/** A fast queue implementation using a dynamic array and ring buffer as its backbone
	*  This container keeps the queue data together for data locality
	*/
	template<typename T>
	class Queue
	{
		T* queueItems;
		uint32_t head;
		uint32_t tail;

		MemoryAllocator& allocator;
		const uint32_t maxItems;
	public:
		/** Construct a queue providing the memory allocator and max items allowed
		 *  @param allocator The memory allocator for the queue to use
		 *  @param maxItems The maximum number of items the queue can hold
		*/
		explicit Queue(MemoryAllocator& allocator, const uint32_t maxItems)
			: queueItems(nullptr), head(0), tail(0), allocator(allocator), maxItems(maxItems)
		{
			queueItems = static_cast<T*>(allocator.Allocate(sizeof(T), maxItems));
		}

		/** Destroys the queue instance and frees any allocated memory
		*/
		~Queue()
		{
			allocator.Free(queueItems);
		}

		/** Enqueue an item into the queue
		*  @param item The item to enqueue
		*/
		void Enqueue(T& item)
		{
			assert((tail + 1) % maxItems != head);
			queueItems[tail] = item;
			tail = (tail + 1) % maxItems;
		}

		/** Dequeue an item from the queue
		*  @return The item dequeued from the queue
		*/
		T& Dequeue()
		{
			assert(head != tail);
			T& item = queueItems[head];
			head = (head + 1) % maxItems;
			return item;
		}

		/** Checks if the queue is empty or not
		*  @return True or false indicating whether the queue is empty or not
		*/
		bool IsEmpty() const
		{
			return (head == tail);
		}
	};
}


#endif // VIPER_QUEUE_HEADER