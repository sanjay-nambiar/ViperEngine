#ifndef SINGLETON_HEADER
#define SINGLETON_HEADER

#include <cassert>
#include "MemoryManager.h"

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
		friend T;
		Singleton()
		{
			assert(sInstance == nullptr);
		};

		~Singleton() = default;

		static T* sInstance;
		static MemoryManager* allocator;
	public:
		/** Creates the singleton instance of the child class
		* @param allocator The memory allocator to use for getting heap memory
		*/
		static void CreateInstance(MemoryManager* allocator)
		{
			assert(sInstance == nullptr);
			sInstance = static_cast<T*>(allocator->Allocate(sizeof(T), 1));
		}

		/** Get singleton instance of the child class
		* @return The singleton instance of the child class
		*/
		static T* GetInstance()
		{
			assert(sInstance != nullptr);
			return sInstance;
		}

		/** Destroys the singleton instance
		* Frees the memory allocated for the singleton instance
		*/
		static void Destroy()
		{
			assert(sInstance != nullptr);
			assert(allocator != nullptr);
			allocator->Free(sInstance);
			sInstance = nullptr;
			allocator = nullptr;
		}
	};

	template <typename T> T* Singleton<T>::sInstance(nullptr);
	template <typename T> MemoryManager* Singleton<T>::allocator(nullptr);
}

#endif // SINGLETON_HEADER
