#pragma once

#include <utility>
#include <vector>
#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		class MeshAsset;
		class MaterialAsset;

		class ModelAsset : public Asset
		{
		public:
			ModelAsset(const StringID& assetFullName);
			~ModelAsset() = default;

			const std::vector<std::pair<const MeshAsset*, const MaterialAsset*>>& Meshes() const;
		private:
			std::vector<std::pair<const MeshAsset*, const MaterialAsset*>> meshes;
		};
	}
}
