#include "Pch.h"

using namespace std;
using namespace glm;
using namespace Assimp;
using namespace Viper;
using namespace Viper::Asset;

namespace ModelPipeline
{
	ModelAsset* ModelProcessor::LoadModel(const string& filename, bool flipUVs)
	{
		auto modelAsset = new ModelAsset(filename);
		auto& modelData = modelAsset->Data();
		Importer importer;
		uint32_t flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder | aiProcess_CalcTangentSpace;

		if (flipUVs)
		{
			flags |= aiProcess_FlipUVs;
		}

		const aiScene* scene = importer.ReadFile(filename, flags);
		if (scene == nullptr)
		{
			throw GameException(importer.GetErrorString());
		}

		auto modelName = Utility::GetFilenameWithoutExtension(filename);
		if (scene->HasMaterials())
		{
			for (uint32_t i = 0; i < scene->mNumMaterials; i++)
			{
				auto materialAsset = MaterialProcessor::LoadMaterial(*(scene->mMaterials[i]), modelName + "-" 
					+ to_string(modelData.materials.size()) + ".vmat");
				if (materialAsset != nullptr)
				{
					modelData.materials.push_back(materialAsset);
				}
			}
		}

		if (scene->HasMeshes())
		{
			for (uint32_t i = 0; i < scene->mNumMeshes; i++)
			{
				auto& assimpMesh = *(scene->mMeshes[i]);
				auto meshAsset = MeshProcessor::LoadMesh(assimpMesh, modelName + "-" + to_string(i) + ".vmsh");
				modelData.meshes.push_back(meshAsset);
				modelData.meshMaterialMap.insert({ i, assimpMesh.mMaterialIndex });
			}
		}
		return modelAsset;
	}
}
