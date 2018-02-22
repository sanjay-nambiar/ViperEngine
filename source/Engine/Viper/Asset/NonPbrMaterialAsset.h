#pragma once

#include "glm/glm.hpp"
#include "Core/Types.h"
#include "MaterialAsset.h"

namespace Viper
{
	namespace Assets
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
			ASSET_DECLARATION(NonPbrMaterialAsset)
			~NonPbrMaterialAsset() = default;

			NonPbrMaterialData& Data();

			bool operator==(const MaterialAsset& rhs) const override;
			bool operator!=(const MaterialAsset& rhs) const override;
		protected:
			virtual const MaterialData& BaseData() const override;
		private:
			void LoadFrom(InputStreamHelper& inputHelper) override;
			void SaveTo(OutputStreamHelper& outputHelper) const override;

			NonPbrMaterialData data;
		};
	}
}
