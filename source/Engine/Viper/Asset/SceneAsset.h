#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Assets
	{
		class SceneAsset : public Asset
		{
		public:
			ASSET_DECLARATION(SceneAsset)
			~SceneAsset() = default;

		private:
			void LoadFrom(InputStreamHelper& inputHelper) override;
			void SaveTo(OutputStreamHelper& outputHelper) const override;
		};
	}
}
