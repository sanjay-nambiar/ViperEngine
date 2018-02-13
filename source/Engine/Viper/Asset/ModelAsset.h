#pragma once

#include <unordered_map>
#include <vector>
#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		class MeshAsset;
		class MaterialAsset;

		struct ModelData
		{
			std::vector<MeshAsset*> meshes;
			std::vector<MaterialAsset*> materials;
			std::unordered_map<uint32_t, uint32_t> meshMaterialMap;
		};

		class ModelAsset : public Asset
		{
		public:
			ModelAsset(const StringID& assetFullName);
			~ModelAsset() = default;

			ModelData& Data();

			void Load(InputStreamHelper& inputHelper) override;
			void Save(OutputStreamHelper& outputHelper) const override;

			bool operator==(const ModelAsset& rhs) const;
			bool operator!=(const ModelAsset& rhs) const;
		private:
			ModelData data;
		};
	}
}
