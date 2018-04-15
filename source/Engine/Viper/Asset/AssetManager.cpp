#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		const string AssetManager::DefaultAssetExtension = ".vasset";
		const string AssetManager::DefaultContentDirectory = "Content\\";
		const string AssetManager::DefaultRegistryFile = "AssetRegistry.bin";

		unordered_map<AssetType, AssetConstructor>& AssetManager::Constructors()
		{
			static unordered_map<AssetType, AssetConstructor> constructors(static_cast<uint32_t>(AssetType::AssetTypes));
			return constructors;
		}

		AssetManager::AssetManager(Memory::MemoryAllocator& allocator) :
			Service(ServiceType::AssetManager), allocator(allocator)
		{
		}

		void AssetManager::Initialize(const string& contentDirectory, const string& registryFileName)
		{
			ifstream assetRegistryFile;
			registry.Data().contentDirectory = contentDirectory;
			registry.Data().registryFileName = registryFileName;
			registry.Load();
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
				auto& package = registryData.packages.at(assetMeta.packageId);

				ifstream file;
				OpenInputFile(file, registryData.contentDirectory + package.packageFile.ToString() + DefaultAssetExtension);
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
			auto& package = registryData.packages.at(assetMeta.packageId);
			ofstream file;
			OpenOutputFile(file, registryData.contentDirectory + package.packageFile.ToString() + DefaultAssetExtension);
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
				OpenInputFile(file, registryData.contentDirectory + package.packageFile.ToString() + DefaultAssetExtension);
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

		const unordered_map<StringID, Asset*>& AssetManager::LoadedAssets()
		{
			return loadedAssets;
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
			if (loadedAssets.find(assetMeta.assetId) != loadedAssets.end())
			{
				return loadedAssets.at(assetMeta.assetId);
			}
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
