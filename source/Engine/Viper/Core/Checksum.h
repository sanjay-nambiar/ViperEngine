#pragma once

#include "Types.h"

namespace Viper
{
	class Checksum
	{
	public:
		Checksum() = default;

		template <typename ChecksumFunctor>
		static std::uint32_t Compute(const std::string& buffer);

		template <typename ChecksumFunctor>
		static std::uint32_t Compute(const char* buffer, std::int32_t length);
	};

	class CRC32
	{
	public:
		std::uint32_t operator()(const char* buffer, std::uint32_t length);
	};
}

#define VIPER_CHECKSUM
#include "Checksum.inl"
