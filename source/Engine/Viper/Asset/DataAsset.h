#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Assets
	{
		class DataAsset : public Asset
		{
		public:
			DataAsset(StringID& assetFullName);
			~DataAsset() = default;

		private:
			void LoadFrom(InputStreamHelper& inputHelper) override;
			void SaveTo(OutputStreamHelper& outputHelper) const override;
		};
	}
}
