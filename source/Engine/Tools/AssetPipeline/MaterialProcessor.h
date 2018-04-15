#pragma once

#include <string>
#include <unordered_map>
#include "Asset/NonPbrMaterialAsset.h"
#include "Asset/PbrMaterialAsset.h"
#include "Asset/TextureAsset.h"
#include "AssetProcessor.h"
#include "Core/Types.h"

struct aiMaterial;

namespace AssetPipeline
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

	class TextureProcessor;

    class MaterialProcessor
    {
    public:
		MaterialProcessor(AssetProcessor& processor, TextureProcessor& textureProcessor);
		MaterialProcessor(const MaterialProcessor&) = delete;
		MaterialProcessor(MaterialProcessor&&) = delete;

		Viper::Assets::MaterialAsset* LoadMaterial(const Resource& resource, aiMaterial& material, std::uint32_t index = 0);
	private:
		Viper::Assets::MaterialAsset* LoadPbrMaterial(const Resource& resource, const Viper::StringID& assetId, aiMaterial& material);
		Viper::Assets::MaterialAsset* LoadNonPbrMaterial(const Resource& resource, const  Viper::StringID& assetId, aiMaterial& material);
        
		AssetProcessor& assetProcessor;
		TextureProcessor& textureProcessor;
		
		static void InitializeTextureTypeMappings();
        static std::unordered_map<TextureType, std::uint32_t> sTextureTypeMappings;
    };
}
