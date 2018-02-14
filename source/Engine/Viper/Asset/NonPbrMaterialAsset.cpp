#include "Viper.h"
#include "NonPbrMaterialAsset.h"

using namespace glm;
using namespace std;

namespace Viper
{
	namespace Asset
	{
		NonPbrMaterialData::NonPbrMaterialData() :
			ambientMap(nullptr), diffuseMap(nullptr), specularMap(nullptr),
			specularPowerMap(nullptr), opacityMap(nullptr)
		{
			isPbr = false;
		}

		NonPbrMaterialAsset::NonPbrMaterialAsset(const StringID& assetFullName) :
			MaterialAsset(assetFullName)
		{
		}

		void NonPbrMaterialAsset::Load(InputStreamHelper& inputHelper)
		{
			inputHelper >> data.isPbr;
			inputHelper >> data.ambient;
			inputHelper >> data.diffuse;
			inputHelper >> data.specular;
			inputHelper >> data.specularPower;
			data.normalMap = LoadTextureHelper(inputHelper);
			data.ambientMap = LoadTextureHelper(inputHelper);
			data.diffuseMap = LoadTextureHelper(inputHelper);
			data.specularMap = LoadTextureHelper(inputHelper);
			data.specularPowerMap = LoadTextureHelper(inputHelper);
			data.opacityMap = LoadTextureHelper(inputHelper);
		}

		void NonPbrMaterialAsset::Save(OutputStreamHelper& outputHelper) const
		{
			outputHelper << data.isPbr;
			outputHelper << data.ambient;
			outputHelper << data.diffuse;
			outputHelper << data.specular;
			outputHelper << data.specularPower;
			SaveTextureHelper(data.normalMap, outputHelper);
			SaveTextureHelper(data.ambientMap, outputHelper);
			SaveTextureHelper(data.diffuseMap, outputHelper);
			SaveTextureHelper(data.specularMap, outputHelper);
			SaveTextureHelper(data.specularPowerMap, outputHelper);
			SaveTextureHelper(data.opacityMap, outputHelper);
		}

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
	}
}
