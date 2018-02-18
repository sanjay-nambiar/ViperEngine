#include "Viper.h"
#include "ModelAsset.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		ModelAsset::ModelAsset(const StringID& assetFullName) :
			Asset(assetFullName, AssetType::Model)
		{
		}

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
			// Load materials
			uint32_t materialCount;
			bool isPbr;
			string tempString;
			inputHelper >> materialCount;
			data.materials.reserve(materialCount);
			for (uint32_t i = 0; i < materialCount; ++i)
			{
				inputHelper >> tempString;
				inputHelper >> isPbr;
				MaterialAsset* material;
				if (isPbr)
				{
					material = new PbrMaterialAsset(StringID(tempString));
				}
				else
				{
					material = new NonPbrMaterialAsset(StringID(tempString));
				}
				material->Load();
				data.materials.push_back(material);
			}

			uint32_t meshCount, materialIndex;
			inputHelper >> meshCount;
			data.meshes.reserve(meshCount);
			for (uint32_t i = 0; i < meshCount; ++i)
			{
				inputHelper >> tempString;
				inputHelper >> materialIndex;
				auto mesh = new MeshAsset(StringID(tempString));
				mesh->Load();
				data.meshes.push_back(mesh);
				data.meshMaterialMap.insert({ i, materialIndex });
			}
		}

		void ModelAsset::SaveTo(OutputStreamHelper& outputHelper) const
		{
			// Serialize materials
			outputHelper << static_cast<uint32_t>(data.materials.size());
			for (auto& material : data.materials)
			{
				outputHelper << material->AssetFullName().ToString();
				outputHelper << material->IsPbr();
				material->Save();
			}

			// Serialize meshes
			outputHelper << static_cast<uint32_t>(data.meshes.size());
			uint32_t index = 0;
			for (auto& mesh : data.meshes)
			{
				outputHelper << mesh->AssetFullName().ToString();
				mesh->Save();
				outputHelper << data.meshMaterialMap.at(index);
				++index;
			}
		}
	}
}
