#pragma once

#include <cassert>
#include "Memory/MemoryManager.h"

namespace Viper
{
	/** A template base class which helps convert a class to a Singleton.
	 *  A class ChildClass can be converted to Singleton by inheriting from Singleton<ChildClass> and
	 *  creating a private constructor. This class should also be declared as a friend to ChildClass
	 */
	template<typename T>
	class Singleton
	{
	protected:
		// Declare T as a friend
		friend T;

		/** Protected constructor to prevent non derived classes from inheriting
		 */
		Singleton()
		{
			assert(sInstance == nullptr);
		};

		Singleton(const Singleton<T>&) = delete;
		Singleton(Singleton<T>&&) = delete;
		Singleton& operator=(const Singleton<T>&) = delete;
		Singleton& operator=(Singleton<T>&&) = delete;

		static T* sInstance;
		static Memory::MemoryManager* allocator;
	public:
		/** Creates the singleton instance of the child class
		 *  @param allocatorRef The memory allocator to use for getting heap memory
		 */
		static void CreateInstance(Memory::MemoryManager& allocatorRef)
		{
			assert(sInstance == nullptr);
			allocator = &allocatorRef;
			void* memBlock = allocator->Allocate<T>(1);
			assert(memBlock != nullptr);
			sInstance = new(memBlock) T();
		}

		/** Get pointer to the singleton instance of the child class
		 *  @return The pointer to the singleton instance of the child class
		 */
		static T* GetInstancePtr()
		{
			assert(sInstance != nullptr);
			return sInstance;
		}

		/** Get a reference to the singleton instance of the child class
		 *  @return A reference to the singleton instance of the child class
		 */
		static T& GetInstance()
		{
			assert(sInstance != nullptr);
			return (*sInstance);
		}

		/** Destroys the singleton instance
		 *  Frees the memory allocated for the singleton instance
		 */
		static void Destroy()
		{
			assert(sInstance != nullptr);
			assert(allocator != nullptr);
			allocator->Free(sInstance);
			sInstance = nullptr;
			allocator = nullptr;
		}

		~Singleton() = default;
	};

	template <typename T> T* Singleton<T>::sInstance(nullptr);
	template <typename T> Memory::MemoryManager* Singleton<T>::allocator(nullptr);
}
