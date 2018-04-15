#include <functional>

namespace std
{
	template <> struct hash<Viper::StringID>
	{
		size_t operator()(const Viper::StringID& obj) const
		{
			return obj.Hash();
		}
	};

	template<>
	struct equal_to<Viper::StringID>
	{
		bool operator() (const Viper::StringID& data1, const Viper::StringID& data2) const
		{
			return (data1.Hash() == data2.Hash());
		}
	};
}
