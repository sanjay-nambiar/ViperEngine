#ifndef SINGLETON_HEADER
#define SINGLETON_HEADER

#include <cassert>

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
		static void CreateInstance()
		{
			assert(sInstance == nullptr);
			sInstance = new T();
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

	template <typename T> T* Singleton<T>::sInstance = nullptr;
}

#endif // SINGLETON_HEADER
