#include "Viper.h"
#include "DataAsset.h"

namespace Viper
{
	namespace Asset
	{
		DataAsset::DataAsset(StringID& assetFullName) :
			Asset(assetFullName, AssetType::Data)
		{
		}

		void DataAsset::Load(InputStreamHelper&)
		{
		}

		void DataAsset::Save(OutputStreamHelper&) const
		{
		}
	}
}
