#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		unordered_map<AssetType, AssetConstructor>& AssetManager::Constructors()
		{
			static unordered_map<AssetType, AssetConstructor> constructors(static_cast<uint32_t>(AssetType::AssetTypes));
			return constructors;
		}

		AssetManager::AssetManager(Memory::MemoryAllocator& allocator) :
			Service(ServiceType::AssetManager), allocator(allocator)
		{
		}

		void AssetManager::Initialize(const std::string& registryFileName)
		{
			ifstream assetRegistryFile;
			OpenInputFile(assetRegistryFile, registryFileName);
			{
				InputStreamHelper helper(assetRegistryFile);
				ReadAssetRegistry(helper);
			}
			assetRegistryFile.close();
		}

		void AssetManager::Shutdown()
		{
			UnloadAll();
		}

		Asset* AssetManager::LoadSynchronous(const StringID& assetId)
		{
			Asset* asset = nullptr;
			auto preloadedAsset = loadedAssets.find(assetId);
			if (preloadedAsset != loadedAssets.end())
			{
				asset = preloadedAsset->second;
			}
			else
			{
				auto& registryData = registry.Data();
				auto& assetsMap = registryData.assets;
				assert(assetsMap.find(assetId) != assetsMap.end());
				auto& assetMeta = assetsMap.at(assetId.Hash());

				ifstream file;
				OpenInputFile(file, registryData.contentDirectory + assetMeta.packageId.ToString());
				{
					InputStreamHelper helper(file);
					asset = LoadAsset(helper, assetMeta);
				}
			}
			return asset;
		}

		void AssetManager::UnloadSynchronous(const StringID& assetId)
		{
			assert(loadedAssets.find(assetId.Hash()) != loadedAssets.end());
			auto asset = loadedAssets.at(assetId.Hash());
			loadedAssets.erase(assetId.Hash());
			delete asset;
		}

		void AssetManager::SaveSynchronous(const Asset& asset)
		{
			auto& registryData = registry.Data();
			auto assetMeta = registryData.assets.at(asset.AssetId());
			auto& packageId = assetMeta.packageId;
			ofstream file;
			OpenOutputFile(file, registryData.contentDirectory + packageId.ToString());
			OutputStreamHelper helper(file);
			helper << asset.Type();
			asset.SaveTo(helper);
		}

		void AssetManager::LoadAll()
		{
			auto& registryData = registry.Data();
			for (auto& packageEntry : registryData.packages)
			{
				auto& package = packageEntry.second;
				ifstream file;
				OpenInputFile(file, registryData.contentDirectory + package.packageId.ToString());
				{
					InputStreamHelper helper(file);
					for (auto& assetId : package.assets)
					{
						auto& assetMeta = registryData.assets.at(assetId);
						LoadAsset(helper, assetMeta);
					}
				}
			}
		}

		void AssetManager::UnloadAll()
		{
			for (auto asset : loadedAssets)
			{
				loadedAssets.erase(asset.first);
				delete asset.second;
			}
		}

		AssetRegistry& AssetManager::Registry()
		{
			return registry;
		}

		void AssetManager::ReadAssetRegistry(InputStreamHelper& helper)
		{
			auto& registryData = registry.Data();
			// Read asset metadata
			{
				uint32_t assetsCount;
				helper >> assetsCount;
				string tempName;
				registryData.assets.reserve(assetsCount);
				for (uint32_t i = 0; i < assetsCount; ++i)
				{
					helper >> tempName;
					StringID assetId(tempName);
					registryData.assets.insert({ assetId, AssetRegistry::AssetMeta(assetId) });
				}
			}
			// Read package metadata
			{
				uint32_t packagesCount;
				helper >> packagesCount;

				StringID assetId(0);
				uint32_t assetOffset;
				registryData.packages.reserve(packagesCount);
				std::string tempString;
				for (uint32_t i = 0; i < packagesCount; ++i)
				{
					helper >> tempString;
					StringID tempId(tempString);
					registryData.packages.insert({ tempId, AssetRegistry::PackageMeta(tempId) });
					auto& packageMeta = registryData.packages.at(tempId);

					uint32_t assetCount;
					helper >> assetCount;
					packageMeta.assets.reserve(assetCount);
					for (uint32_t j = 0; j < assetCount; ++i)
					{
						helper >> assetId;
						helper >> assetOffset;
						auto& assetMeta = registryData.assets.at(assetId);
						assetMeta.offset = assetOffset;
						assetMeta.packageId = packageMeta.packageId;
						assetMeta.indexInPackage = static_cast<uint32_t>(packageMeta.assets.size());
						packageMeta.assets.push_back(assetMeta.assetId);
					}
				}
			}
		}

		void AssetManager::SaveAssetRegistry(OutputStreamHelper& helper)
		{
			auto& registryData = registry.Data();
			// Write asset Ids
			{
				helper << static_cast<uint32_t>(registryData.assets.size());
				for (auto& assetEntry : registryData.assets)
				{
					// write string and not ID
					helper << assetEntry.first.ToString();
				}
			}
			// Write package names and offset info
			{
				helper << static_cast<uint32_t>(registryData.packages.size());
				for (auto& packageEntry : registryData.packages)
				{
					auto& package = packageEntry.second;
					// write string and not ID
					helper << package.packageId.ToString();
					helper << static_cast<uint32_t>(package.assets.size());
					for (auto& assetId : package.assets)
					{
						auto& asset = registryData.assets.at(assetId);
						helper << assetId;
						helper << asset.offset;
					}
				}
			}
		}

		void AssetManager::OpenInputFile(ifstream& file, const std::string& filename)
		{
			file.open(filename, ios::binary | ios::in);
			if (!file.is_open())
			{
				throw GameException("Unable to open file : " + filename);
			}
		}

		void AssetManager::OpenOutputFile(ofstream& file, const std::string& filename)
		{
			file.open(filename, ios::binary | ios::out);
			if (!file.is_open())
			{
				throw GameException("Unable to open file : " + filename);
			}
		}

		Asset* AssetManager::LoadAsset(InputStreamHelper& helper, const AssetRegistry::AssetMeta& assetMeta)
		{
			helper.Stream().seekg(assetMeta.offset, ios_base::beg);
			AssetType assetType;
			helper >> assetType;
			auto asset = Constructors().at(assetType)(assetMeta.assetId);
			loadedAssets.insert({ assetMeta.assetId.Hash(), asset });
			asset->LoadFrom(helper);
			return asset;
		}
	}
}
