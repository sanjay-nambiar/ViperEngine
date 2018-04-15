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

		bool DataAsset::operator==(const Asset&) const
		{
			return true;
		}

		bool DataAsset::operator!=(const Asset& rhs) const
		{
			return !(*this == rhs);
		}
	}
}
