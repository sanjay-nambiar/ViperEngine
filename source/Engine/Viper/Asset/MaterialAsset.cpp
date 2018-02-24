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

		TextureAsset* MaterialAsset::LoadTextureHelper(InputStreamHelper& helper)
		{
			bool tempBool;
			string tempString;
			helper >> tempBool;
			if (tempBool)
			{
				helper >> tempString;
				return static_cast<TextureAsset*>(assetManager.LoadSynchronous(StringID(tempString)));
			}
			return nullptr;
		}

		void MaterialAsset::SaveTextureHelper(TextureAsset* textureAsset, OutputStreamHelper& helper) const
		{
			helper << (textureAsset != nullptr);
			if (textureAsset != nullptr)
			{
				const auto& textureFileName = textureAsset->AssetFullName().ToString();
				auto rawFilename = Utility::GetFilenameWithoutExtension(textureFileName);
				textureAsset->Save();
				helper << rawFilename;
			}
		}
	}
}
