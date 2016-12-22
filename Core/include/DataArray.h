#ifndef DATA_ARRAY_HEADER
#define DATA_ARRAY_HEADER

#include <algorithm>
#include <cassert>
#include <functional>
#include "MemoryManager.h"
#include "Types.h"


#define MAX_ITEMS_ALLOWED	65535


namespace Viper
{
	/** A fast and efficient fixed size dynamic array based container. 
	*  This container keeps the data together to ensure data locality. 
	*  Returns stable ids to data items using slot mapping. 
	*  Uses move semantics to move around data efficiently
	*/
	template <typename T>
	class DataArray<T>
	{
	public:
		/** Struct to hold data for a single element in the container
		*/
		struct DataItem
		{
			/** The data value
			*/
			T data;
			/** The unique id of the data element
			*/
			uint32_t id;
		};	

		/** Creates a data array container. 
		*  @param count The max number of elements the array can hold
		*  @param allocator The memory allocator to use for dynamic memory allocation
		*/
		explicit DataArray<T>(uint32_t count, MemoryManager& allocator)
			: allocator(allocator), maxCount(count), items(nullptr),  slots(nullptr),
			freeSlots(nullptr), count(0), freeSlotsTop(count-1), key(0)
		{
			assert(count > 0);
			assert(count <= MAX_ITEMS_ALLOWED);
			items = static_cast<DataItem*>(allocator.Allocate(sizeof(DataItem), maxCount));

			uint32_t* memory = static_cast<uint32_t*>(allocator.Allocate(sizeof(uint32_t), maxCount*2));
			slots = memory;
			freeSlots = memory + (sizeof(uint32_t) * maxCount);

			for(uint32_t i = 0; i < maxCount; i++)
			{
				freeSlots[maxCount - 1 - i] = i;
			}
		}

		/** Destroys the container and frees all allocated memory
		*/
		~DataArray()
		{
			allocator.Free(items);
			allocator.Free(slots);
		}

		/** Adds a new entry into the array and returns the newly added item
		*  @param data The data to add to the array
		*/
		DataItem& Add(T& data)
		{
			assert(count < maxCount);
			
			uint32_t index = freeSlots[freeSlotsTop];			// index to slots array is obtained from freeSlots array's top
			slots[index] = count;								// new object is always added to the end of the items array
			index |= (key++ << 16);								// the id of the object is calculated as (key << 16) | index to slots array
			items[count].id = index;
			items[count].data = std::move(data);
			freeSlotsTop--;
			return items[count++];
		}

		/** Removes an item with the given id from the container
		*  @param id The id of the data element to be removed
		*/
		void Remove(uint32_t id)
		{
			uint32_t index = slots[id & 0xFFFF];
			DataItem& item = items[index];
			assert(item.id == id);
			items[index] = std::move(items[count-1]);
			count--;
			freeSlots[++freeSlotsTop] = (id & 0xFFFF);
		}

		/** Get the item with the given id from the container
		*  @param id The id of the data element to be retrieved
		*  @return The element with the given id
		*/
		DataItem& Get(uint32_t id)
		{
			uint32_t index = slots[id & 0xFFFF];
			T& item = items[index];
			assert(item.id == id);
			return item;
		}

		/** Get the dynamic array of elements
		*  @return A pointer to the array of elements
		*/
		DataItem* GetArray()
		{
			return items;
		}

		/** Sort the array based on a predicate
		*  @param predicate The predicate based on which the array is to be sorted
		*/
		void Sort(std::function<bool(const T&, const T&)> predicate)
		{
			std::sort(items, &items[count], predicate);
		}
	private:
		// Allocator set during init. Used to allocate and free memory
		MemoryManager& allocator;
		// maximum items allowed
		uint32_t maxCount;

		// The item array storing the data
		DataItem* items;
		// indirection indices
		uint32_t* slots;
		// free list
		uint32_t* freeSlots;

		// num alloced items
		uint32_t count;
		// free slots top
		uint32_t freeSlotsTop;

		// unique key value
		uint32_t key;
	};
}

#endif // DATA_ARRAY_HEADER
