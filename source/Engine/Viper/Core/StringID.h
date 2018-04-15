#pragma once

#include <string>
#include <unordered_map>
#include "StreamHelper.h"

namespace Viper
{
	class StringID
	{
	public:
		StringID();
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
#if _DEBUG
		std::string* stringVal;
#endif
	};

	OutputStreamHelper& operator<<(OutputStreamHelper& helper, const StringID& value);
	InputStreamHelper& operator>>(InputStreamHelper& helper, StringID& value);
}

#include "StringID.inl"
