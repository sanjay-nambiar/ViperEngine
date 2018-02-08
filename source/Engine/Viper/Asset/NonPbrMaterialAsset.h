#pragma once

#include "glm/glm.hpp"
#include "MaterialAsset.h"

namespace Viper
{
	namespace Asset
	{
		class NonPbrMaterialAsset : public MaterialAsset
		{
		public:
			NonPbrMaterialAsset(const StringID& assetFullName);
			~NonPbrMaterialAsset() = default;

			const glm::vec3& Ambient() const;
			const glm::vec3& Diffuse() const;
			const glm::vec3& Specular() const;
			float32_t SpecularPower() const;
			const TextureAsset* AmbientMap() const;
			const TextureAsset* DiffuseMap() const;
			const TextureAsset* SpecularMap() const;
			const TextureAsset* SpecularPowerMap() const;
			const TextureAsset* OpacityMap() const;
		private:
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float32_t specularPower;
			const TextureAsset* ambientMap;
			const TextureAsset* diffuseMap;
			const TextureAsset* specularMap;
			const TextureAsset* specularPowerMap;
			const TextureAsset* opacityMap;
		};
	}
}
