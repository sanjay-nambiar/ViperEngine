#include "Viper.h"
#include "ModelAsset.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		ASSET_DEFINITION(ModelAsset, Asset, AssetType::Model)

		bool ModelAsset::operator==(const ModelAsset& rhs) const
		{
			if (data.meshes.size() != rhs.data.meshes.size())
			{
				return false;
			}
			for (uint32_t index = 0; index < data.meshes.size(); ++index)
			{
				if (*data.meshes[index] != *rhs.data.meshes[index])
				{
					return false;
				}
			}
			if (data.materials.size() != rhs.data.materials.size())
			{
				return false;
			}
			for (uint32_t index = 0; index < data.materials.size(); ++index)
			{
				if (*data.materials[index] != *rhs.data.materials[index])
				{
					return false;
				}
			}
			if (data.meshMaterialMap != rhs.data.meshMaterialMap)
			{
				return false;
			}
			return true;
		}

		bool ModelAsset::operator!=(const ModelAsset& rhs) const
		{
			return !(*this == rhs);
		}

		ModelData& ModelAsset::Data()
		{
			return data;
		}

		void ModelAsset::LoadFrom(InputStreamHelper& inputHelper)
		{
			// Load materials meta
			uint32_t materialCount;
			StringID materialId;
			vector<StringID> materialIds;
			inputHelper >> materialCount;
			data.materials.reserve(materialCount);
			for (uint32_t i = 0; i < materialCount; ++i)
			{
				inputHelper >> materialId;
				materialIds.push_back(materialId);
			}

			// Load meshes meta
			uint32_t meshCount, materialIndex;
			StringID meshId;
			vector<StringID> meshIds;
			inputHelper >> meshCount;
			data.meshes.reserve(meshCount);
			for (uint32_t i = 0; i < meshCount; ++i)
			{
				inputHelper >> meshId;
				inputHelper >> materialIndex;
				data.meshMaterialMap.insert({ i, materialIndex });
			}

			// Load material data
			for (auto& id : materialIds)
			{
				auto material = static_cast<MaterialAsset*>(assetManager.LoadSynchronous(id));
				data.materials.push_back(material);
			}

			// Load mesh data
			for (auto& id : meshIds)
			{
				auto mesh = static_cast<MeshAsset*>(assetManager.LoadSynchronous(id));
				data.meshes.push_back(mesh);
			}
		}

		void ModelAsset::SaveTo(OutputStreamHelper& outputHelper) const
		{
			// Serialize material meta
			outputHelper << static_cast<uint32_t>(data.materials.size());
			for (auto& material : data.materials)
			{
				outputHelper << material->AssetId();
			}

			// Serialize meshes meta
			outputHelper << static_cast<uint32_t>(data.meshes.size());
			uint32_t index = 0;
			for (auto& mesh : data.meshes)
			{
				outputHelper << mesh->AssetId();
				outputHelper << data.meshMaterialMap.at(index);
				++index;
			}

			// Serialize material data
			for (auto& material : data.materials)
			{
				material->Save();
			}

			// Serialize mesh data
			for (auto& mesh : data.meshes)
			{
				mesh->Save();
			}
		}
	}
}
