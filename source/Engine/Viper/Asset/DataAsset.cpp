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
	}
}
