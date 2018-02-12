#pragma once

#include "MaterialAsset.h"
#include "glm/glm.hpp"

namespace Viper
{
	namespace Asset
	{
		struct PbrMaterialData final : public MaterialData
		{
			glm::vec3 albedo;
			float32_t metallic;
			float32_t roughness;
			float32_t ao;
			TextureAsset* albedoMap;
			TextureAsset* metallicMap;
			TextureAsset* roughnessMap;
			TextureAsset* aoMap;

			PbrMaterialData();
			~PbrMaterialData() = default;
		};

		class PbrMaterialAsset : public MaterialAsset
		{
		public:
			PbrMaterialAsset(const StringID& assetFullName);
			~PbrMaterialAsset() = default;

			PbrMaterialData& Data();

			void Load(InputStreamHelper& inputHelper) override;
			void Save(OutputStreamHelper& outputHelper) const override;
		protected:
			const MaterialData& BaseData() const override;
		private:
			PbrMaterialData data;
		};
	}
}
