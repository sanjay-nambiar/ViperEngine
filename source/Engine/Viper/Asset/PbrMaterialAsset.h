#pragma once

#include "MaterialAsset.h"
#include "glm/glm.hpp"

namespace Viper
{
	namespace Asset
	{
		class PbrMaterialAsset : public MaterialAsset
		{
		public:
			PbrMaterialAsset(const StringID& assetFullName);
			~PbrMaterialAsset() = default;

			const glm::vec3& Albedo() const;
			float32_t Metallic() const;
			float32_t Roughness() const;
			float32_t AmbientOcclusion() const;
			const TextureAsset* AlbedoMap() const;
			const TextureAsset* MetallicMap() const;
			const TextureAsset* RoughnessMap() const;
			const TextureAsset* AmbientOcclusionMap() const;
		private:
			glm::vec3 albedo;
			float32_t metallic;
			float32_t roughness;
			float32_t ao;
			const TextureAsset* albedoMap;
			const TextureAsset* metallicMap;
			const TextureAsset* roughnessMap;
			const TextureAsset* aoMap;
		};
	}
}
