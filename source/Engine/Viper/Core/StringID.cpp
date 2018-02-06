#include "Pch.h"

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

	StringID::StringID(uint32_t hash) :
		hash(hash)
	{
		assert(HashToStringLookup().find(hash) != HashToStringLookup().end());
	}
	
	StringID::StringID(const std::string& literal) :
		hash(Checksum::Compute<CRC32>(literal))
	{
		HashToStringLookup()[hash] = literal;
	}

	const string& StringID::ToString()
	{
		return HashToStringLookup()[hash];
	}

	bool StringID::operator==(const StringID& other)
	{
		return (hash == other.hash);
	}

	bool StringID::operator!=(const StringID& other)
	{
		return (hash != other.hash);
	}
}