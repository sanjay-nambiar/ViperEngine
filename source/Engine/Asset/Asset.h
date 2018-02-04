#pragma once

#include "Core/StringID.h"

namespace Viper
{
	namespace Asset
	{
		enum class AssetType
		{
			Texture,
			Mesh,
			Model,
			Level,
			Data
		};

		class Asset
		{
			Asset(const StringID& assetFullName);
		private:
			StringID assetFullName;
		};
	}
}
