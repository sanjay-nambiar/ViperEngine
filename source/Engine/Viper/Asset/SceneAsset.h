#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Assets
	{
		class SceneAsset : public Asset
		{
		public:
			SceneAsset(StringID& assetFullName);
			~SceneAsset() = default;

		private:
			void LoadFrom(InputStreamHelper& inputHelper) override;
			void SaveTo(OutputStreamHelper& outputHelper) const override;
		};
	}
}
