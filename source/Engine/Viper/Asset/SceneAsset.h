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

			bool operator==(const Asset& rhs) const override;
			bool operator!=(const Asset& rhs) const override;

		private:
			void LoadFrom(InputStreamHelper& inputHelper) override;
			void SaveTo(OutputStreamHelper& outputHelper) const override;
		};
	}
}
