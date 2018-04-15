#include "Viper.h"
#include "AssetRegistry.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		bool AssetRegistry::AssetMeta::operator==(const AssetRegistry::AssetMeta& rhs) const
		{
			return (assetId == rhs.assetId) && (packageId == rhs.packageId) && (indexInPackage == rhs.indexInPackage) && (offset == rhs.offset);
		}

		bool AssetRegistry::PackageMeta::operator==(const AssetRegistry::PackageMeta& rhs) const
		{
			return (packageId == rhs.packageId) && (packageFile == rhs.packageFile) && (assets == rhs.assets);
		}

		bool AssetRegistry::RegistryData::operator==(const RegistryData& rhs) const
		{
			return (assets == rhs.assets) && (packages == rhs.packages) && (contentDirectory == rhs.contentDirectory);
		}

		AssetRegistry::AssetMeta::AssetMeta(const StringID& assetId) :
			assetId(assetId), packageId(0), indexInPackage(0), offset(0)
		{
		}

		AssetRegistry::PackageMeta::PackageMeta(const StringID& packageId, const StringID& packageFile, uint32_t assetCount) :
			packageId(packageId), packageFile(packageFile)
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

		void AssetRegistry::Save(bool isDebug)
		{
			const string filename = data.contentDirectory + data.registryFileName;
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
				helper << package.packageId.ToString();
				helper << package.packageFile.ToString();
				helper << static_cast<uint32_t>(package.assets.size());
				for (auto& assetId : package.assets)
				{
					auto& asset = data.assets.at(assetId);
					helper << assetId.ToString();
					helper << asset.indexInPackage;
					helper << asset.offset;
				}
			}
			if (isDebug)
			{
				SaveDebug();
			}
		}

		void AssetRegistry::Load()
		{
			const string filename = data.contentDirectory + data.registryFileName;
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
				string packageFile;
				helper >> packageFile;
				PackageMeta package(packageId, packageFile);
				uint32_t assetsCount;
				helper >> assetsCount;
				package.assets.reserve(assetsCount);
				for (uint32_t assetIndex = 0; assetIndex < assetsCount; ++assetIndex)
				{
					string assetId;
					helper >> assetId;
					AssetMeta asset(assetId);
					helper >> asset.indexInPackage;
					helper >> asset.offset;
					asset.packageId = package.packageId;
					package.assets.push_back(asset.assetId);
					data.assets.insert({ asset.assetId, asset });
				}
				data.packages.insert({ package.packageId, package });
			}
		}

		void AssetRegistry::SaveDebug()
		{
			const string filename = data.contentDirectory + data.registryFileName + ".debug";
			ofstream file(filename);
			if (!file)
			{
				throw GameException("Unable to save file: " + filename);
			}

			string prefix = "\t";
			file << "{" << endl;
			file << prefix << "\"content\": \"" << data.contentDirectory << "\"," << endl;
			file << prefix << "\"packageCount\": " << data.packages.size() << "," << endl;
			file << prefix << "\"packages\": [" << endl;
			prefix = "\t\t";
			for (auto& packageEntry : data.packages)
			{
				auto& package = packageEntry.second;
				file << prefix << "{" << endl;
				prefix = "\t\t\t";
				file << prefix << "\"name\": \"" << package.packageId.ToString() << "\"," << endl;
				file << prefix << "\"file\": \"" << package.packageFile.ToString() << "\"," << endl;
				file << prefix << "\"assetCount\": " << package.assets.size() << "," << endl;
				file << prefix << "\"assets\": [" << endl;
				prefix = "\t\t\t\t";
				for (auto& assetId : package.assets)
				{
					auto& asset = data.assets.at(assetId);
					file << prefix << "{" << endl;
					prefix = "\t\t\t\t\t";
					file << prefix << "\"name\": \"" << assetId.ToString() << "\"," << endl;
					file << prefix << "\"index\": " << asset.indexInPackage << "," << endl;
					file << prefix << "\"offset\": " << asset.offset << "," << endl;
					prefix = "\t\t\t\t";
					file << prefix << "}," << endl;
				}
				prefix = "\t\t";
				file << prefix << "}," << endl;
			}
			prefix = "\t";
			file << prefix << "]" << endl;
			file << "}";
		}
	}
}
