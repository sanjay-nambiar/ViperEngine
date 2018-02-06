#pragma once

#include "MaterialAsset.h"
#include "glm/glm.hpp"

namespace Viper
{
	namespace Asset
	{
		class NonPbrMaterialAsset : public MaterialAsset
		{
		public:
			NonPbrMaterialAsset(StringID assetFullName);
			~NonPbrMaterialAsset() = default;

			const glm::vec3& Ambient() const;
			const glm::vec3& Diffuse() const;
			const glm::vec3& Specular() const;
			float SpecularPower() const;
			const TextureAsset* AmbientMap() const;
			const TextureAsset* DiffuseMap() const;
			const TextureAsset* SpecularMap() const;
			const TextureAsset* SpecularPowerMap() const;
			const TextureAsset* OpacityMap() const;
		private:
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float specularPower;
			const TextureAsset* ambientMap;
			const TextureAsset* diffuseMap;
			const TextureAsset* specularMap;
			const TextureAsset* specularPowerMap;
			const TextureAsset* opacityMap;
		};
	}
}
