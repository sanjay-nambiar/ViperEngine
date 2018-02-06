#include "Pch.h"
#include "PbrMaterialAsset.h"

namespace Viper
{
	namespace Asset
	{
		PbrMaterialAsset::PbrMaterialAsset(StringID assetFullName) :
			MaterialAsset(assetFullName), albedoMap(nullptr), metallicMap(nullptr),
			roughnessMap(nullptr), aoMap(nullptr), metallic(0.0f), roughness(0.0f), ao(0.0f)
		{
		}

		const glm::vec3& PbrMaterialAsset::Albedo() const
		{
			return albedo;
		}

		float PbrMaterialAsset::Metallic() const
		{
			return metallic;
		}

		float PbrMaterialAsset::Roughness() const
		{
			return roughness;
		}

		float PbrMaterialAsset::AmbientOcclusion() const
		{
			return ao;
		}
		
		const TextureAsset* PbrMaterialAsset::AlbedoMap() const
		{
			return albedoMap;
		}

		const TextureAsset* PbrMaterialAsset::MetallicMap() const
		{
			return metallicMap;
		}

		const TextureAsset* PbrMaterialAsset::RoughnessMap() const
		{
			return roughnessMap;
		}

		const TextureAsset* PbrMaterialAsset::AmbientOcclusionMap() const
		{
			return aoMap;
		}
	}
}
