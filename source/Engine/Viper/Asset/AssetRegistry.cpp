#include "Viper.h"
#include "AssetRegistry.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		const string AssetRegistry::RegistryData::AssetExtension = ".vasset";

		AssetRegistry::AssetMeta::AssetMeta(const StringID& assetId) :
			assetId(assetId), packageId(0), indexInPackage(0), offset(0)
		{
		}

		AssetRegistry::PackageMeta::PackageMeta(const StringID& packageId, uint32_t assetCount) :
			packageId(packageId)
		{
			if (assetCount > 0)
			{
				assets.reserve(assetCount);
			}
		}

		AssetRegistry::RegistryData& AssetRegistry::Data()
		{
			return data;
		}
	}
}
