#include "Viper.h"

namespace Viper
{
	namespace Assets
	{
		ASSET_DEFINITION(DataAsset, Asset, AssetType::Data)

		void DataAsset::LoadFrom(InputStreamHelper&)
		{
		}

		void DataAsset::SaveTo(OutputStreamHelper&) const
		{
		}
	}
}
