#include "Viper.h"
#include "PbrMaterialAsset.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		PbrMaterialData::PbrMaterialData() :
			albedoMap(nullptr), metallicMap(nullptr), roughnessMap(nullptr), aoMap(nullptr),
			metallic(0.0f), roughness(0.0f), ao(0.0f)
		{
			isPbr = true;
		}

		ASSET_DEFINITION(PbrMaterialAsset, MaterialAsset, AssetType::PbrMaterial)

		PbrMaterialData& PbrMaterialAsset::Data()
		{
			return data;
		}

		bool PbrMaterialAsset::operator==(const MaterialAsset& rhs) const
		{
			if (!rhs.IsPbr())
			{
				return false;
			}
			auto& rhsMaterial = static_cast<const PbrMaterialAsset&>(rhs);
			return ((data.albedo == rhsMaterial.data.albedo) &&
				(data.metallic == rhsMaterial.data.metallic) &&
				(data.roughness == rhsMaterial.data.roughness) &&
				(data.ao == rhsMaterial.data.ao) &&
				IS_TEXTURE_EQUAL(data.normalMap, rhsMaterial.data.normalMap) &&
				IS_TEXTURE_EQUAL(data.albedoMap, rhsMaterial.data.albedoMap) &&
				IS_TEXTURE_EQUAL(data.metallicMap, rhsMaterial.data.metallicMap) &&
				IS_TEXTURE_EQUAL(data.roughnessMap, rhsMaterial.data.roughnessMap) &&
				IS_TEXTURE_EQUAL(data.aoMap, rhsMaterial.data.aoMap));
		}

		bool PbrMaterialAsset::operator!=(const MaterialAsset& rhs) const
		{
			return !(*this == rhs);
		}

		const MaterialData& PbrMaterialAsset::BaseData() const
		{
			auto baseData = static_cast<const MaterialData*>(&data);
			return *baseData;
		}

		void PbrMaterialAsset::LoadFrom(InputStreamHelper& inputHelper)
		{
			inputHelper >> data.albedo;
			inputHelper >> data.metallic;
			inputHelper >> data.roughness;
			inputHelper >> data.ao;
			StringID normalMapId = LoadTextureMetaHelper(inputHelper);
			StringID albedoMapId = LoadTextureMetaHelper(inputHelper);
			StringID metallicMapId = LoadTextureMetaHelper(inputHelper);
			StringID roughnessMapId = LoadTextureMetaHelper(inputHelper);
			StringID aoMapId = LoadTextureMetaHelper(inputHelper);
			StringID emptyId;
			if (normalMapId != emptyId) data.normalMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (albedoMapId != emptyId) data.albedoMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (metallicMapId != emptyId) data.metallicMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (roughnessMapId != emptyId) data.roughnessMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
			if (aoMapId != emptyId) data.aoMap = static_cast<TextureAsset*>(assetManager.LoadSynchronous(normalMapId));
		}

		void PbrMaterialAsset::SaveTo(OutputStreamHelper& outputHelper) const
		{
			outputHelper << data.isPbr;
			outputHelper << data.albedo;
			outputHelper << data.metallic;
			outputHelper << data.roughness;
			outputHelper << data.ao;
			SaveTextureMetaHelper(data.normalMap, outputHelper);
			SaveTextureMetaHelper(data.albedoMap, outputHelper);
			SaveTextureMetaHelper(data.metallicMap, outputHelper);
			SaveTextureMetaHelper(data.roughnessMap, outputHelper);
			SaveTextureMetaHelper(data.aoMap, outputHelper);
			if (data.normalMap) data.normalMap->Save();
			if (data.albedoMap) data.albedoMap->Save();
			if (data.metallicMap) data.metallicMap->Save();
			if (data.roughnessMap) data.roughnessMap->Save();
			if (data.aoMap) data.aoMap->Save();
		}
	}
}
