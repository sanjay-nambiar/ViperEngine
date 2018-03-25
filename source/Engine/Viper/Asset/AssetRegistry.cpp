#include "Viper.h"
#include "AssetRegistry.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		const string AssetRegistry::AssetExtension = ".vasset";
		const string AssetRegistry::AssetRegistryFile = "AssetRegistry.bin";

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

		void AssetRegistry::Load(const string& contentDirectoryRelative)
		{
			data.contentDirectory = contentDirectoryRelative;
			const string filename = data.contentDirectory + AssetRegistryFile;
			ifstream file(filename, ios::binary);
			if (!file)
			{
				throw GameException("Unable to load file: " + filename);
			}
			InputStreamHelper helper(file);
			data.packages.clear();
			data.assets.clear();

			uint32_t packagesCount;
			helper >> data.contentDirectory;
			helper >> packagesCount;
			data.packages.reserve(packagesCount);
			for (uint32_t packageIndex = 0; packageIndex < packagesCount; ++packageIndex)
			{
				string packageId;
				helper >> packageId;
				PackageMeta package(packageId);
				uint32_t assetsCount;
				helper >> assetsCount;
				package.assets.reserve(assetsCount);
				for (uint32_t assetIndex = 0; assetIndex < packagesCount; ++assetIndex)
				{
					string assetId;
					helper >> assetId;
					AssetMeta asset(assetId);
					helper >> asset.indexInPackage;
					helper >> asset.offset;
					asset.packageId = package.packageId;
					package.assets.push_back(asset.assetId);
				}
			}
		}

		void AssetRegistry::Save()
		{
			const string filename = data.contentDirectory + AssetRegistryFile;
			ofstream file(filename, ios::binary);
			if (!file)
			{
				throw GameException("Unable to save file: " + filename);
			}
			OutputStreamHelper helper(file);
			helper << data.contentDirectory;
			helper << static_cast<uint32_t>(data.packages.size());
			for (auto& packageEntry : data.packages)
			{
				auto& package = packageEntry.second;
				helper << packageEntry.first.ToString();
				helper << static_cast<uint32_t>(package.assets.size());
				for (auto& assetId : package.assets)
				{
					auto& asset = data.assets.at(assetId);
					helper << assetId.ToString();
					helper << asset.indexInPackage;
					helper << asset.offset;
				}
			}
		}
	}
}
