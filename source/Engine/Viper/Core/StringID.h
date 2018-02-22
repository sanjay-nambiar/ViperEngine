#pragma once

#include <string>
#include <unordered_map>

namespace Viper
{
	class StringID
	{
	public:
		StringID(std::uint32_t hash);
		StringID(const std::string& literal);

		StringID(const StringID& rhs);
		StringID& operator=(const StringID& rhs);

		const std::string& ToString() const;
		const std::uint32_t Hash() const;
		bool operator==(const StringID& other) const;
		bool operator!=(const StringID& other) const;
	private:
		std::uint32_t hash;
	};
}
