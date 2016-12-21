#ifndef DATA_ARRAY_HEADER
#define DATA_ARRAY_HEADER

#include <cassert>
#include "Allocator.h"
#include "Types.h"

#define MAX_ITEMS_ALLOWED	65535
//
//namespace Viper
//{
//	template <typename T>
//	class DataArray<T>
//	{
//		struct Item
//		{
//			T item;
//			// (key << 16 | index) for alloced entries, (0 | nextFreeIndex) for free list entries
//			uint32_t id;
//		};
//
//		Allocator& allocator;
//		Item* items;
//
//		// total size
//		const uint32_t maxSize;
//		// highest index ever alloced
//		uint32_t maxUsed;
//		// num alloced items
//		uint32_t count;
//		// [1..2^16] (don't let == 0)
//		uint32_t nextKey;
//		// index of first free entry
//		uint32_t freeHead;
//
//	public:
//		explicit DataArray<T>(uint32_t count, MemoryManager* allocator)
//			: allocator(allocator), items(nullptr), maxSize(count), maxUsed(0), count(0), nextKey(1), freeHead(0)
//		{
//			assert(count > 0);
//			assert(count <= MAX_ITEMS_ALLOWED);			
//			items = static_cast<Item*>(allocator->Allocate(sizeof(Item), maxSize));
//		}
//
//		// free memory
//		void Dispose()
//		{
//			items.reset();
//		}
//
//		// resets data members, (runs destructors* on outstanding items, *optional)
//		void Clear()
//		{
//			
//		}
//
//		// alloc (memclear* and/or construct*, *optional) an item from freeList or items[maxUsed++], sets id to (nextKey++ << 16) | index
//		T& Add();
//
//		// puts entry on free list (uses id to store next)
//		void Remove(T&);
//
//		// accessor to the id part if Item
//		uint32_t GetID(T&);
//
//		// return item[id & 0xFFFF]; 
//		T& Get(id);
//		// validates id, then returns item, returns null if invalid.  for cases like AI references and others where 'the thing might have been deleted out from under me'
//		std::shared_ptr<T> TryToGet(id);
//
//		// return next item where id & 0xFFFF0000 != 0 (ie items not on free list)
//		bool Next(T*&);	
//	};
//}

#endif // DATA_ARRAY_HEADER
