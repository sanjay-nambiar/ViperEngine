#ifndef SINGLETON_HEADER
#define SINGLETON_HEADER

#include <cassert>
#include "MemoryManager.h"

namespace Viper
{
	template<typename T>
	class Singleton
	{
		friend T;
		Singleton()
		{
			assert(sInstance == nullptr);
		};

		~Singleton() = default;

		static T* sInstance;
	public:
		static void CreateInstance(MemoryManager& allocator)
		{
			assert(sInstance == nullptr);
			sInstance = static_cast<T*>(allocator.Allocate(sizeof(T), 1));
		}

		static T* GetInstance()
		{
			assert(sInstance != nullptr);
			return sInstance;
		}

		static void Destroy()
		{
			assert(sInstance != nullptr);
			delete sInstance;
			sInstance = nullptr;
		}
	};

	template <typename T> T* Singleton<T>::sInstance(nullptr);
}

#endif // SINGLETON_HEADER
