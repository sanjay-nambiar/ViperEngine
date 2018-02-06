#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		class DataAsset : public Asset
		{
		public:
			DataAsset(StringID& assetFullName);
			~DataAsset() = default;
		};
	}
}
