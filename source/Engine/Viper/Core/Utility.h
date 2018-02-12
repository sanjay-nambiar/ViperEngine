#pragma once

#include <string>

namespace Viper
{
	class Utility
	{
	public:
		static std::string GetFilenameWithoutExtension(const std::string& fullFilename);
	};
}