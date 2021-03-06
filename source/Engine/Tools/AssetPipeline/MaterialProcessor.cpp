#include "Pch.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;
using namespace Viper;
using namespace Viper::Assets;
using namespace AssetPipeline;

namespace AssetPipeline
{
	unordered_map<TextureType, uint32_t> MaterialProcessor::sTextureTypeMappings;

	MaterialProcessor::MaterialProcessor(AssetProcessor& processor, TextureProcessor& textureProcessor)
		: assetProcessor(processor), textureProcessor(textureProcessor)
	{
	}

	MaterialAsset* MaterialProcessor::LoadMaterial(const Resource& resource, aiMaterial& material, std::uint32_t index)
	{
		// Ignore default material
		aiString name;
		material.Get(AI_MATKEY_NAME, name);
		if (string(AI_DEFAULT_MATERIAL_NAME) == string(name.C_Str()))
		{
			return nullptr;
		}
		InitializeTextureTypeMappings();

		auto assetId = StringID(resource.packageName + ":material." + to_string(index));
		cout << "Loading material : " << assetId.ToString() << " ... " << endl;
		Asset* asset = assetProcessor.GetLoadedAsset(assetId);
		if (asset != nullptr)
		{
			cout << "(cache retrieve) ... DONE" << endl;
			return static_cast<MaterialAsset*>(asset);
		}

		auto materialAsset = LoadNonPbrMaterial(resource, assetId, material);
		assetProcessor.RegisterOffset(*materialAsset, resource.packageName, resource.packageFile);
		cout << "Loaded : " << assetId.ToString() << endl;
		return materialAsset;
	}

	MaterialAsset* MaterialProcessor::LoadPbrMaterial(const Resource&, const StringID& assetId, aiMaterial&)
	{
		return new PbrMaterialAsset(assetId);
	}

	MaterialAsset* MaterialProcessor::LoadNonPbrMaterial(const Resource& resource, const StringID& assetId, aiMaterial& material)
	{
		auto materialAsset = new NonPbrMaterialAsset(assetId);
		auto& materialData = materialAsset->Data();

		aiColor3D color(0.0f, 0.0f, 0.0f);
		material.Get(AI_MATKEY_COLOR_AMBIENT, color);
		materialData.ambient = glm::vec3(color.r, color.g, color.b);

		color = aiColor3D(0.0f, 0.0f, 0.0f);
		material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
		materialData.diffuse = glm::vec3(color.r, color.g, color.b);

		color = aiColor3D(0.0f, 0.0f, 0.0f);
		material.Get(AI_MATKEY_COLOR_SPECULAR, color);
		materialData.specular = glm::vec3(color.r, color.g, color.b);

		float shininess = 0.0f;
		material.Get(AI_MATKEY_SHININESS, shininess);
		materialData.specularPower = shininess;

		
		for (TextureType textureType = static_cast<TextureType>(0); textureType < TextureType::Invalid;
			textureType = static_cast<TextureType>(static_cast<int>(textureType) + 1))
		{
			aiTextureType mappedTextureType = static_cast<aiTextureType>(sTextureTypeMappings[textureType]);
			uint32_t textureCount = material.GetTextureCount(mappedTextureType);
			if (textureCount > 0)
			{
				assert(textureCount == 1);
				aiString aiPath;
				if (material.GetTexture(mappedTextureType, 0, &aiPath) == AI_SUCCESS)
				{
					Resource textureResource = resource;
					if (textureResource.packageFile != resource.packageFile)
					{
						cout << "What!!" << endl;
					}
					assetProcessor.GetResource(textureResource, aiPath.C_Str(), resource.resourceDir);

					auto texture = textureProcessor.LoadTexture(textureResource);
					switch (textureType)
					{
					case TextureType::Diffuse:
						materialData.diffuseMap = texture;
						break;
					case TextureType::Specular:
						materialData.specularMap = texture;
						break;
					case TextureType::Ambient:
						materialData.ambientMap = texture;
						break;
					case TextureType::Normal:
						materialData.normalMap = texture;
						break;
					case TextureType::SpecularPower:
						materialData.specularPowerMap = texture;
						break;
					case TextureType::Opacity:
						materialData.opacityMap = texture;
						break;
					default:
						break;
					}
				}
			}
		}
		return materialAsset;
	}

	void MaterialProcessor::InitializeTextureTypeMappings()
	{
		if (sTextureTypeMappings.size() != static_cast<uint32_t>(TextureType::Invalid))
		{
			sTextureTypeMappings[TextureType::Diffuse] = aiTextureType_DIFFUSE;
			sTextureTypeMappings[TextureType::Specular] = aiTextureType_SPECULAR;
			sTextureTypeMappings[TextureType::Ambient] = aiTextureType_AMBIENT;
			sTextureTypeMappings[TextureType::Height] = aiTextureType_HEIGHT;
			sTextureTypeMappings[TextureType::Normal] = aiTextureType_NORMALS;
			sTextureTypeMappings[TextureType::SpecularPower] = aiTextureType_SHININESS;
			sTextureTypeMappings[TextureType::Displacement] = aiTextureType_DISPLACEMENT;
			sTextureTypeMappings[TextureType::Opacity] = aiTextureType_OPACITY;
			sTextureTypeMappings[TextureType::Reflection] = aiTextureType_REFLECTION;
			sTextureTypeMappings[TextureType::Emissive] = aiTextureType_EMISSIVE;
		}
	}
}
