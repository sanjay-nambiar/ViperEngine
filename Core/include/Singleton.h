#ifndef SINGLETON_HEADER
#define SINGLETON_HEADER

#include <cassert>
#include <memory>

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

		static std::shared_ptr<T> sInstance;
	public:
		static void CreateInstance()
		{
			assert(sInstance == nullptr);
			sInstance = std::shared_ptr<T>(new T());
		}

		static std::shared_ptr<T> GetInstance()
		{
			assert(sInstance != nullptr);
			return sInstance;
		}

		static void Destroy()
		{
			assert(sInstance != nullptr);
			sInstance.reset();
			sInstance = nullptr;
		}
	};

	template <typename T> std::shared_ptr<T> Singleton<T>::sInstance(nullptr);
}

#endif // SINGLETON_HEADER
