
namespace Viper
{
	namespace Memory
	{
		template<typename T>
		T* MemoryAllocator::Allocate(std::uint32_t count)
		{
			return reinterpret_cast<T*>(AllocateInternal(sizeof(T), count));
		}
	}
}
