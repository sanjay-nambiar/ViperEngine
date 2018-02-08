#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include "Core/Types.h"
#include "Memory/MemoryAllocator.h"

#define MAX_ITEMS_ALLOWED		65535
#define ID_TO_SLOT(id)			(id & 0xFFFF)
#define CREATE_ID(key, index)	((key << 16) | index)

namespace Viper
{
	namespace Containers
	{
		/** A fast and efficient fixed size dynamic array based container.
		*  This container keeps the data together to ensure data locality.
		*  Returns stable ids to data items using slot mapping.
		*  Uses move semantics to move around data efficiently
		*  The sizeof T should be greater than or equal to sizeof uint32_t
		*  The T type should support default constructor and then and Init method which actually intialize the object
		*/
		template <typename T>
		class DataArray
		{
		public:
			/**
			 * Used to return a reference to the data and it's ID while creating a new entry in the array
			 */
			struct DataItem
			{
				T data;
				uint32_t id;

				DataItem() = default;

				DataItem(DataItem&& other) noexcept : data(other.data), id(other.id)
				{}

				DataItem(DataItem& other) noexcept : data(other.data), id(other.id)
				{}

				DataItem& operator=(DataItem&& other) noexcept
				{
					if (this != &other)
					{
						data = other.data;
						id = other.id;
					}
					return (*this);
				}

				DataItem& operator=(DataItem& other) noexcept
				{
					if (this != &other)
					{
						data = other.data;
						id = other.id;
					}
					return (*this);
				}
			};

			/** The predicate function type that can be used to sort the array
			 * Create a custom function with this type to define custom sort behaviour
			 */
			typedef std::function<bool(const DataItem&, const DataItem&)> PredicateFunction;

			/** Creates a data array container. T should support move semantics
			*  @param maxItems The max number of elements the array can hold
			*  @param allocator The memory allocator to use for dynamic memory allocation
			*/
			explicit DataArray<T>(uint32_t maxItems, Memory::MemoryAllocator& allocator)
				: allocator(allocator), maxCount(maxItems), items(nullptr),
				slots(nullptr), nextFreeSlot(0), count(0), key(0)
			{
				assert(maxItems > 0);
				assert(maxItems <= MAX_ITEMS_ALLOWED);
				items = static_cast<DataItem*>(allocator.Allocate(sizeof(DataItem), maxCount));
				memset(items, 0, sizeof(DataItem) * maxCount);
				slots = static_cast<SlotMapItem*>(allocator.Allocate(sizeof(SlotMapItem), maxCount));

				for (uint32_t i = 0; i < maxCount - 1; i++)
				{
					slots[i].nextFreeSlot = i + 1;
				}
				slots[maxCount - 1].nextFreeSlot = -1;
			}

			/** Destroys the container and frees all allocated memory
			*/
			~DataArray()
			{
				allocator.Free(items);
				allocator.Free(slots);
			}

			/** Adds a new entry into the array and returns the newly added other reference
			*  @return The DataItem object with reference to newly created data and it's id
			*/
			DataItem& Create()
			{
				assert(count < maxCount);
				uint32_t itemIndex = count;
				uint32_t slotIndex = nextFreeSlot;
				uint32_t id = CREATE_ID(key++, slotIndex);
				items[itemIndex].id = id;
				count++;
				nextFreeSlot = slots[slotIndex].nextFreeSlot;
				slots[slotIndex].itemIndex = itemIndex;
				return items[itemIndex];
			}

			/** Removes an entry with the given id from the container
			*  @param id The id of the data element to be removed
			*/
			void Remove(uint32_t id)
			{
				uint32_t slotIndex = ID_TO_SLOT(id);
				uint32_t dataIndex = slots[slotIndex].itemIndex;
				assert(items[dataIndex].id == id);
				items[dataIndex] = std::move(items[count - 1]);
				slots[ID_TO_SLOT(items[dataIndex].id)].itemIndex = dataIndex;
				count--;
				slots[slotIndex].nextFreeSlot = nextFreeSlot;
				nextFreeSlot = slotIndex;
			}

			/** Get the other with the given id from the container
			*  @param id The id of the data element to be retrieved
			*  @return The element with the given id
			*/
			DataItem& Get(uint32_t id)
			{
				uint32_t index = slots[ID_TO_SLOT(id)].itemIndex;
				assert(items[index].id == id);
				return items[index];
			}

			/** Get the dynamic array of elements
			*  @return A pointer to the array of elements
			*/
			T* GetArray()
			{
				return items;
			}

			/** Get the number of items
			*  @return The number of items in the container
			*/
			uint32_t GetSize() const
			{
				return count;
			}

			/** Sort the array based on a predicate
			*  @param predicate The predicate based on which the array is to be sorted
			*/
			void Sort(PredicateFunction predicate)
			{
				std::sort(items, &items[count], predicate);
				for (uint32_t dataIndex = 0; dataIndex < count; dataIndex++)
				{
					slots[ID_TO_SLOT(items[dataIndex].id)].itemIndex = dataIndex;
				}
			}
		private:
			// Stores either an index to the other list or if free, index to next free slot
			union SlotMapItem
			{
				int32_t itemIndex;
				int32_t nextFreeSlot;
			};

			// Allocator set during init. Used to allocate and free memory
			Memory::MemoryAllocator& allocator;
			// maximum items allowed
			uint32_t maxCount;

			// The other array storing the data
			DataItem* items;
			// indirection indices
			SlotMapItem* slots;
			// next available slot
			int32_t nextFreeSlot;
			// num alloced items
			uint32_t count;

			// unique key value
			uint32_t key;
		};
	}
}
// Undef the symbols which are meant for only this file
#undef MAX_ITEMS_ALLOWED
#undef ID_TO_SLOT
#undef CREATE_ID
