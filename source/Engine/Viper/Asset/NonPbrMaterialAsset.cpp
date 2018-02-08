#include "Pch.h"
#include "NonPbrMaterialAsset.h"

using namespace glm;

namespace Viper
{
	namespace Asset
	{
		NonPbrMaterialAsset::NonPbrMaterialAsset(const StringID& assetFullName) :
			MaterialAsset(assetFullName), ambientMap(nullptr), diffuseMap(nullptr),
			specularMap(nullptr), specularPowerMap(nullptr), opacityMap(nullptr)
		{
		}

		const vec3& NonPbrMaterialAsset::Ambient() const
		{
			return ambient;
		}

		const vec3& NonPbrMaterialAsset::Diffuse() const
		{
			return diffuse;
		}

		const vec3& NonPbrMaterialAsset::Specular() const
		{
			return specular;
		}

		float NonPbrMaterialAsset::SpecularPower() const
		{
			return specularPower;
		}

		const TextureAsset* NonPbrMaterialAsset::AmbientMap() const
		{
			return ambientMap;
		}

		const TextureAsset* NonPbrMaterialAsset::DiffuseMap() const
		{
			return diffuseMap;
		}

		const TextureAsset* NonPbrMaterialAsset::SpecularMap() const
		{
			return specularMap;
		}

		const TextureAsset* NonPbrMaterialAsset::SpecularPowerMap() const
		{
			return specularPowerMap;
		}

		const TextureAsset* NonPbrMaterialAsset::OpacityMap() const
		{
			return opacityMap;
		}
	}
}
