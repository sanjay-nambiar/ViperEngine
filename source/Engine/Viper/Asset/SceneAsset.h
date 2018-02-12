#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		class SceneAsset : public Asset
		{
		public:
			SceneAsset(StringID& assetFullName);
			~SceneAsset() = default;

			void Load(InputStreamHelper& inputHelper) override;
			void Save(OutputStreamHelper& outputHelper) const override;
		};
	}
}
