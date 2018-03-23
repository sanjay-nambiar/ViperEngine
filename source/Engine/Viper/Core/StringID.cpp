#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace StringID_Private
	{
		static unordered_map<uint32_t, string>& HashToStringLookup()
		{
			static unordered_map<uint32_t, string> lookup;
			return lookup;
		}
	}

	using namespace StringID_Private;

	StringID::StringID() :
		StringID("")
	{
	}

	StringID::StringID(uint32_t hash) :
		hash(hash),
#if _DEBUG
		stringVal(&HashToStringLookup()[hash])
#endif
	{
	}
	
	StringID::StringID(const std::string& literal) :
		hash(Checksum::Compute<CRC32>(literal))
	{
		assert(HashToStringLookup().find(hash) == HashToStringLookup().end() || HashToStringLookup()[hash] == literal);
		HashToStringLookup()[hash] = literal;
#if _DEBUG
		stringVal = &HashToStringLookup()[hash];
#endif
	}

	StringID::StringID(const StringID& rhs) :
		StringID(rhs.hash)
	{
	}

	StringID& StringID::operator=(const StringID& rhs)
	{
		hash = rhs.hash;
#if _DEBUG
		stringVal = rhs.stringVal;
#endif
		return *this;
	}

	const string& StringID::ToString() const
	{
		return HashToStringLookup()[hash];
	}

	const std::uint32_t StringID::Hash() const
	{
		return hash;
	}

	bool StringID::operator==(const StringID& other) const
	{
		return (hash == other.hash);
	}

	bool StringID::operator!=(const StringID& other) const
	{
		return (hash != other.hash);
	}

	OutputStreamHelper& operator<<(OutputStreamHelper& helper, const StringID& value)
	{
		return helper << value.Hash();
	}

	InputStreamHelper& operator>>(InputStreamHelper& helper, StringID& value)
	{
		uint32_t hash;
		helper >> hash;
		value = hash;
		return helper;
	}
}