#pragma once

#include <string>
#include <unordered_map>
#include "Asset/NonPbrMaterialAsset.h"
#include "Asset/PbrMaterialAsset.h"
#include "Asset/TextureAsset.h"

struct aiMaterial;

namespace ModelPipeline
{
	enum class TextureType
	{
		Diffuse,
		Specular,
		Ambient,
		Height,
		Normal,
		SpecularPower,
		Displacement,
		Opacity,
		Reflection,
		Emissive,
		Invalid
	};

    class MaterialProcessor
    {
    public:
		MaterialProcessor() = delete;

		static Viper::Assets::MaterialAsset* LoadMaterial(aiMaterial& material, const std::string& assetFullName);
	private:
		static Viper::Assets::MaterialAsset* LoadPbrMaterial(aiMaterial& material, const std::string& assetFullName);
		static Viper::Assets::MaterialAsset* LoadNonPbrMaterial(aiMaterial& material, const std::string& assetFullName);
        static void InitializeTextureTypeMappings();
        static std::unordered_map<TextureType, std::uint32_t> sTextureTypeMappings;
    };
}
