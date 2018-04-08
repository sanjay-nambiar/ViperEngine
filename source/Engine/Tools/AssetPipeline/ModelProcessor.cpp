#include "Pch.h"

using namespace std;
using namespace glm;
using namespace Assimp;
using namespace Viper;
using namespace Viper::Assets;

namespace AssetPipeline
{
	ModelProcessor::ModelProcessor(AssetProcessor& assetProcessor, MeshProcessor& meshProcessor, MaterialProcessor& materialProcessor)
		: assetProcessor(assetProcessor), meshProcessor(meshProcessor), materialProcessor(materialProcessor)
	{
	}

	ModelAsset* ModelProcessor::LoadModel(const Resource& resource, std::uint32_t index, bool flipUVs)
	{
		auto assetId = StringID(resource.packageName + ":model." + to_string(index));
		cout << "Loading model : " << assetId.ToString() << " ... " << endl;
		Asset* asset = assetProcessor.GetLoadedAsset(assetId);
		if (asset != nullptr)
		{
			cout << "(cache retrieve) ... DONE" << endl;
			return static_cast<ModelAsset*>(asset);
		}

		auto modelAsset = new ModelAsset(assetId);
		assetProcessor.RegisterOffset(*modelAsset, resource.packageName, resource.packageFile);
		auto& modelData = modelAsset->Data();
		Importer importer;
		uint32_t flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder | aiProcess_CalcTangentSpace;

		if (flipUVs)
		{
			flags |= aiProcess_FlipUVs;
		}

		const aiScene* scene = importer.ReadFile(resource.resourceFile, flags);
		if (scene == nullptr)
		{
			throw GameException(importer.GetErrorString());
		}

		uint32_t defaultMaterialIndex = scene->mNumMaterials;
		if (scene->HasMaterials())
		{
			for (uint32_t i = 0; i < scene->mNumMaterials; i++)
			{
				auto materialAsset = materialProcessor.LoadMaterial(resource, *(scene->mMaterials[i]), i);
				if (materialAsset != nullptr)
				{
					modelData.materials.push_back(materialAsset);
				}
				else
				{
					defaultMaterialIndex = i;
				}
			}
		}

		if (scene->HasMeshes())
		{
			for (uint32_t i = 0; i < scene->mNumMeshes; i++)
			{
				auto& assimpMesh = *(scene->mMeshes[i]);
				auto meshAsset = meshProcessor.LoadMesh(resource, assimpMesh, i);
				modelData.meshes.push_back(meshAsset);
				if (assimpMesh.mMaterialIndex > defaultMaterialIndex)
				{
					modelData.meshMaterialMap.insert({ i, assimpMesh.mMaterialIndex - 1 });
				}
				else
				{
					modelData.meshMaterialMap.insert({ i, assimpMesh.mMaterialIndex });
				}
			}
		}
		cout << "Loaded model : " << assetId.ToString() << endl;
		return modelAsset;
	}
}
