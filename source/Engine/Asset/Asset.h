#pragma once

#include "Core/StringID.h"

namespace Viper
{
	namespace Asset
	{
		enum class AssetType
		{
			Invalid,
			Texture,
			Mesh,
			Material,
			Model,
			Scene,
			Data
		};

		class Asset
		{
		public:
			Asset(const StringID& assetFullName, AssetType type = AssetType::Invalid);
			virtual ~Asset() = 0;

			const StringID& AssetFullName() const;
			AssetType Type() const;
		protected:
			StringID assetFullName;
			AssetType type;
		};
	}
}
