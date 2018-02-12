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
		}

		NonPbrMaterialData& NonPbrMaterialAsset::Data()
		{
			return data;
		}

		const MaterialData& NonPbrMaterialAsset::BaseData() const
		{
			auto baseData = static_cast<const MaterialData*>(&data);
			return *baseData;
		}
	}
}
