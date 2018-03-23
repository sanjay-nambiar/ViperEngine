#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		MaterialData::MaterialData() :
			isPbr(false), normalMap(nullptr)
		{
		}

		MaterialAsset::MaterialAsset(const StringID& assetFullName, AssetType type) :
			Asset(assetFullName, type)
		{
		}

		bool MaterialAsset::IsPbr() const
		{
			return BaseData().isPbr;
		}

		StringID MaterialAsset::LoadTextureMetaHelper(InputStreamHelper& helper)
		{
			bool tempBool;
			StringID textureId;
			helper >> tempBool;
			if (tempBool)
			{
				helper >> textureId;
			}
			return textureId;
		}

		bool MaterialAsset::SaveTextureMetaHelper(TextureAsset* textureAsset, OutputStreamHelper& helper) const
		{
			helper << (textureAsset != nullptr);
			if (textureAsset != nullptr)
			{
				helper << textureAsset->AssetId();
			}
			return (textureAsset != nullptr);
		}
	}
}
