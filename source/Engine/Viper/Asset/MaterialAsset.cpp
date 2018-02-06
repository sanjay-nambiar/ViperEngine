#include "Pch.h"
#include "MaterialAsset.h"

namespace Viper
{
	namespace Asset
	{
		MaterialAsset::MaterialAsset(StringID assetFullName) :
			Asset(assetFullName, AssetType::Material), normalMap(nullptr)
		{
		}

		const TextureAsset* MaterialAsset::NormalMap() const
		{
			return normalMap;
		}
	}
}
