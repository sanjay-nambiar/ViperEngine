#pragma once

#include "Asset.h"
#include <utility>
#include <vector>

namespace Viper
{
	namespace Asset
	{
		class MeshAsset;
		class MaterialAsset;

		class ModelAsset : public Asset
		{
		public:
			ModelAsset(StringID& assetFullName);
			~ModelAsset() = default;

			const std::vector<std::pair<const MeshAsset*, const MaterialAsset*>>& Meshes() const;
		private:
			std::vector<std::pair<const MeshAsset*, const MaterialAsset*>> meshes;
		};
	}
}
