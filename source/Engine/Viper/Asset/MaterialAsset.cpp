#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Asset
	{
		MaterialData::MaterialData() :
			isPbr(false), normalMap(nullptr)
		{
		}

		MaterialAsset::MaterialAsset(const StringID& assetFullName) :
			Asset(assetFullName, AssetType::Material)
		{
		}

		bool MaterialAsset::IsPbr() const
		{
			return BaseData().isPbr;
		}

		TextureAsset* MaterialAsset::LoadTextureHelper(InputStreamHelper& helper)
		{
			bool tempBool;
			string tempString;
			helper >> tempBool;
			if (tempBool)
			{
				helper >> tempString;
				TextureAsset* texture = new TextureAsset(tempString);
				static_cast<Asset*>(texture)->Load();
				return texture;
			}
			return nullptr;
		}

		void MaterialAsset::SaveTextureHelper(TextureAsset* textureAsset, OutputStreamHelper& helper)
		{
			helper << (textureAsset != nullptr);
			if (textureAsset != nullptr)
			{
				const auto& textureFileName = textureAsset->AssetFullName().ToString();
				auto rawFilename = Utility::GetFilenameWithoutExtension(textureFileName) + ".vtex";
				textureAsset->SaveAs(rawFilename);
				helper << rawFilename;
			}
		}
	}
}
