#include "Viper.h"
#include "DataAsset.h"

namespace Viper
{
	namespace Assets
	{
		DataAsset::DataAsset(StringID& assetFullName) :
			Asset(assetFullName, AssetType::Data)
		{
		}

		void DataAsset::LoadFrom(InputStreamHelper&)
		{
		}

		void DataAsset::SaveTo(OutputStreamHelper&) const
		{
		}
	}
}
