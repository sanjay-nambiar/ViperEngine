#pragma once

#include "glm/glm.hpp"
#include "Core/Types.h"
#include "MaterialAsset.h"

namespace Viper
{
	namespace Asset
	{
		struct NonPbrMaterialData final : MaterialData
		{
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float32_t specularPower;
			TextureAsset* ambientMap;
			TextureAsset* diffuseMap;
			TextureAsset* specularMap;
			TextureAsset* specularPowerMap;
			TextureAsset* opacityMap;

			NonPbrMaterialData();
			~NonPbrMaterialData() = default;
		};

		class NonPbrMaterialAsset : public MaterialAsset
		{
		public:
			NonPbrMaterialAsset(const StringID& assetFullName);
			~NonPbrMaterialAsset() = default;

			NonPbrMaterialData& Data();

			void Load(InputStreamHelper& inputHelper) override;
			void Save(OutputStreamHelper& outputHelper) const override;
		protected:
			virtual const MaterialData& BaseData() const override;
		private:
			NonPbrMaterialData data;
		};
	}
}
