#include "Viper.h"
#include "NonPbrMaterialAsset.h"

using namespace glm;
using namespace std;

namespace Viper
{
	namespace Assets
	{
		NonPbrMaterialData::NonPbrMaterialData() :
			ambientMap(nullptr), diffuseMap(nullptr), specularMap(nullptr),
			specularPowerMap(nullptr), opacityMap(nullptr)
		{
			isPbr = false;
		}

		ASSET_DEFINITION(NonPbrMaterialAsset, MaterialAsset, AssetType::NonPbrMaterial)

		NonPbrMaterialData& NonPbrMaterialAsset::Data()
		{
			return data;
		}

		bool NonPbrMaterialAsset::operator==(const MaterialAsset& rhs) const
		{
			if (rhs.IsPbr())
			{
				return false;
			}
			auto& rhsMaterial = static_cast<const NonPbrMaterialAsset&>(rhs);
			return ((data.ambient == rhsMaterial.data.ambient) &&
				(data.diffuse == rhsMaterial.data.diffuse) &&
				(data.specular == rhsMaterial.data.specular) &&
				(data.specularPower == rhsMaterial.data.specularPower) &&
				IS_TEXTURE_EQUAL(data.normalMap, rhsMaterial.data.normalMap) &&
				IS_TEXTURE_EQUAL(data.ambientMap, rhsMaterial.data.ambientMap) &&
				IS_TEXTURE_EQUAL(data.diffuseMap, rhsMaterial.data.diffuseMap) &&
				IS_TEXTURE_EQUAL(data.specularMap, rhsMaterial.data.specularMap) &&
				IS_TEXTURE_EQUAL(data.specularPowerMap, rhsMaterial.data.specularPowerMap) &&
				IS_TEXTURE_EQUAL(data.opacityMap, rhsMaterial.data.opacityMap));
		}

		bool NonPbrMaterialAsset::operator!=(const MaterialAsset& rhs) const
		{
			return !(*this == rhs);
		}

		const MaterialData& NonPbrMaterialAsset::BaseData() const
		{
			auto baseData = static_cast<const MaterialData*>(&data);
			return *baseData;
		}

		void NonPbrMaterialAsset::LoadFrom(InputStreamHelper& inputHelper)
		{
			inputHelper >> data.isPbr;
			inputHelper >> data.ambient;
			inputHelper >> data.diffuse;
			inputHelper >> data.specular;
			inputHelper >> data.specularPower;
			StringID normalMapId = LoadTextureMetaHelper(inputHelper);
			StringID ambientMapId = LoadTextureMetaHelper(inputHelper);
			StringID diffuseMapId = LoadTextureMetaHelper(inputHelper);
			StringID specularMapId = LoadTextureMetaHelper(inputHelper);
			StringID specularPowerMapId = LoadTextureMetaHelper(inputHelper);
			StringID opacityMapId = LoadTextureMetaHelper(inputHelper);
			StringID emptyId;
			if (normalMapId != emptyId) data.normalMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (ambientMapId != emptyId) data.ambientMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (diffuseMapId != emptyId) data.diffuseMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (specularMapId != emptyId) data.specularMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (specularPowerMapId != emptyId) data.specularPowerMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (opacityMapId != emptyId) data.opacityMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
		}

		void NonPbrMaterialAsset::SaveTo(OutputStreamHelper& outputHelper) const
		{
			outputHelper << data.isPbr;
			outputHelper << data.ambient;
			outputHelper << data.diffuse;
			outputHelper << data.specular;
			outputHelper << data.specularPower;
			SaveTextureMetaHelper(data.normalMap, outputHelper);
			SaveTextureMetaHelper(data.ambientMap, outputHelper);
			SaveTextureMetaHelper(data.diffuseMap, outputHelper);
			SaveTextureMetaHelper(data.specularMap, outputHelper);
			SaveTextureMetaHelper(data.specularPowerMap, outputHelper);
			SaveTextureMetaHelper(data.opacityMap, outputHelper);
			if (data.normalMap) data.normalMap->Save();
			if (data.ambientMap) data.ambientMap->Save();
			if (data.diffuseMap) data.diffuseMap->Save();
			if (data.specularMap) data.specularMap->Save();
			if (data.specularPowerMap) data.specularPowerMap->Save();
			if (data.opacityMap) data.opacityMap->Save();
		}
	}
}
