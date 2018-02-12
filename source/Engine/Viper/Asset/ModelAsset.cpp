#include "Viper.h"
#include "ModelAsset.h"

using namespace std;

namespace Viper
{
	namespace Asset
	{
		ModelAsset::ModelAsset(const StringID& assetFullName) :
			Asset(assetFullName, AssetType::Model)
		{
		}

		void ModelAsset::Load(InputStreamHelper& inputHelper)
		{
			// Load materials
			uint32_t materialCount;
			bool isPbr;
			inputHelper >> materialCount;
			data.materials.reserve(materialCount);
			for (uint32_t i = 0; i < materialCount; ++i)
			{
				inputHelper >> isPbr;
				MaterialAsset* material;
				if (isPbr)
				{
					material = new PbrMaterialAsset(StringID(""));
				}
				else
				{
					material = new NonPbrMaterialAsset(StringID(""));
				}
				material->Load(inputHelper);
			}

			// Serialize meshes
			uint32_t meshCount, materialIndex;
			inputHelper >> meshCount;
			data.meshes.reserve(meshCount);
			for (auto& mesh : data.meshes)
			{
				mesh->Load(inputHelper);
				inputHelper >> materialIndex;
				mesh->Data().materialAsset = data.materials.at(materialIndex);
			}
		}

		void ModelAsset::Save(OutputStreamHelper& outputHelper) const
		{
			// Serialize materials
			outputHelper << static_cast<uint32_t>(data.materials.size());
			for (auto& material : data.materials)
			{
				outputHelper << material->IsPbr();
				material->Save(outputHelper);
			}

			// Serialize meshes
			outputHelper << static_cast<uint32_t>(data.meshes.size());
			uint32_t index = 0;
			for (auto& mesh : data.meshes)
			{
				mesh->Save(outputHelper);
				outputHelper << data.meshMaterialMap.at(index);
				++index;
			}
		}

		ModelData& ModelAsset::Data()
		{
			return data;
		}
	}
}
