#include "Viper.h"
#include "PbrMaterialAsset.h"

using namespace std;

namespace Viper
{
	namespace Asset
	{
		PbrMaterialData::PbrMaterialData() :
			albedoMap(nullptr), metallicMap(nullptr), roughnessMap(nullptr), aoMap(nullptr),
			metallic(0.0f), roughness(0.0f), ao(0.0f)
		{
			isPbr = true;
		}

		PbrMaterialAsset::PbrMaterialAsset(const StringID& assetFullName) :
			MaterialAsset(assetFullName)
		{
		}

		void PbrMaterialAsset::Load(InputStreamHelper& inputHelper)
		{
			inputHelper >> data.isPbr;
			inputHelper >> data.albedo;
			inputHelper >> data.metallic;
			inputHelper >> data.roughness;
			inputHelper >> data.ao;
			data.normalMap = LoadTextureHelper(inputHelper);
			data.albedoMap = LoadTextureHelper(inputHelper);
			data.metallicMap = LoadTextureHelper(inputHelper);
			data.roughnessMap = LoadTextureHelper(inputHelper);
			data.aoMap = LoadTextureHelper(inputHelper);
		}

		void PbrMaterialAsset::Save(OutputStreamHelper& outputHelper) const
		{
			outputHelper << data.isPbr;
			outputHelper << data.albedo;
			outputHelper << data.metallic;
			outputHelper << data.roughness;
			outputHelper << data.ao;
			SaveTextureHelper(data.normalMap, outputHelper);
			SaveTextureHelper(data.albedoMap, outputHelper);
			SaveTextureHelper(data.metallicMap, outputHelper);
			SaveTextureHelper(data.roughnessMap, outputHelper);
			SaveTextureHelper(data.aoMap, outputHelper);
		}

		PbrMaterialData& PbrMaterialAsset::Data()
		{
			return data;
		}

		const MaterialData& PbrMaterialAsset::BaseData() const
		{
			auto baseData = static_cast<const MaterialData*>(&data);
			return *baseData;
		}
	}
}
