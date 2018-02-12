#include "Pch.h"

using namespace std;
using namespace Viper;
using namespace Viper::Asset;
using namespace TexturePipeline;

namespace ModelPipeline
{
	unordered_map<TextureType, uint32_t> MaterialProcessor::sTextureTypeMappings;

	MaterialAsset* MaterialProcessor::LoadMaterial(aiMaterial& material)
	{
		InitializeTextureTypeMappings();
		return LoadNonPbrMaterial(material);
	}

	MaterialAsset* MaterialProcessor::LoadPbrMaterial(aiMaterial&)
	{
		return new PbrMaterialAsset(StringID(""));
	}

	MaterialAsset* MaterialProcessor::LoadNonPbrMaterial(aiMaterial& material)
	{
		auto materialAsset = new NonPbrMaterialAsset(StringID(""));
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

		for (TextureType textureType = static_cast<TextureType>(0); textureType < TextureType::Invalid; textureType = static_cast<TextureType>(static_cast<int>(textureType) + 1))
		{
			aiTextureType mappedTextureType = static_cast<aiTextureType>(sTextureTypeMappings[textureType]);
			uint32_t textureCount = material.GetTextureCount(mappedTextureType);
			if (textureCount > 0)
			{
				assert(textureCount == 1);
				aiString path;
				if (material.GetTexture(mappedTextureType, 0, &path) == AI_SUCCESS)
				{
					auto texture = TextureProcessor::LoadTexture(path.C_Str());
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
