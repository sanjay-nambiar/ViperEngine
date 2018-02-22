#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Assets
	{
		class DataAsset : public Asset
		{
		public:
			ASSET_DECLARATION(DataAsset)
			~DataAsset() = default;

		private:
			void LoadFrom(InputStreamHelper& inputHelper) override;
			void SaveTo(OutputStreamHelper& outputHelper) const override;
		};
	}
}
