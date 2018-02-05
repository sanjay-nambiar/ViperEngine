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
			PbrMaterialAsset(StringID assetFullName);
			~PbrMaterialAsset() = default;

			const glm::vec3& Albedo() const;
			float Metallic() const;
			float Roughness() const;
			float AmbientOcclusion() const;
			const TextureAsset* AlbedoMap() const;
			const TextureAsset* MetallicMap() const;
			const TextureAsset* RoughnessMap() const;
			const TextureAsset* AmbientOcclusionMap() const;
		private:
			glm::vec3 albedo;
			float metallic;
			float roughness;
			float ao;
			const TextureAsset* albedoMap;
			const TextureAsset* metallicMap;
			const TextureAsset* roughnessMap;
			const TextureAsset* aoMap;
		};
	}
}
