#pragma once

#ifndef VIPER_CHECKSUM
#include "Checksum.h"
#endif

namespace Viper
{
	template <typename ChecksumFunctor>
	std::uint32_t Checksum::Compute(const std::string& buffer)
	{
		ChecksumFunctor functor;
		return functor(buffer.c_str(), static_cast<uint32_t>(buffer.size()));
	}

	template <typename ChecksumFunctor>
	std::uint32_t Checksum::Compute(const char* buffer, std::int32_t length)
	{
		ChecksumFunctor functor;
		return functor(buffer, length);
	}
}
