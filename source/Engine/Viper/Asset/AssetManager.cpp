#include "Viper.h"

using namespace std;

namespace Viper
{
	namespace Assets
	{
		unordered_map<AssetType, AssetConstructor> AssetManager::Constructors = {};

		AssetManager::AssetManager() :
			Service(ServiceType::AssetManager)
		{
		}

		void AssetManager::Initialize(const std::string& registryFileName)
		{
			ifstream assetRegistryFile;
			OpenFile(assetRegistryFile, registryFileName);
			{
				InputStreamHelper helper(assetRegistryFile);
				AssetRegistry registry;
				ReadAssetRegistry(registry, helper);
				LoadRegistryData(registry);
			}
			assetRegistryFile.close();
		}

		void AssetManager::Shutdown()
		{
			UnloadAll();
		}

		void AssetManager::LoadSynchronous(const StringID& assetId)
		{
			assert(loadedAssets.find(assetId.Hash()) == loadedAssets.end());
			assert(assetMetadata.find(assetId.Hash()) != assetMetadata.end());
			auto& metadata = assetMetadata.at(assetId.Hash());
			
			ifstream file;
			OpenFile(file, metadata.resourceFile.ToString());
			{
				InputStreamHelper helper(file);
				LoadAsset(helper, assetId, metadata.offset);
			}
			file.close();
		}

		void AssetManager::UnloadSynchronous(const StringID& assetId)
		{
			assert(loadedAssets.find(assetId.Hash()) != loadedAssets.end());
			auto asset = loadedAssets.at(assetId.Hash());
			loadedAssets.erase(assetId.Hash());
			delete asset;
		}

		void AssetManager::LoadAll()
		{
			for (auto& assetsEntry : fileMetadata)
			{
				auto& filename = StringID(assetsEntry.first).ToString();
				ifstream file;
				OpenFile(file, filename);
				{
					InputStreamHelper helper(file);
					auto& assets = assetsEntry.second;
					for (auto& assetId : assets)
					{
						auto& metadata = assetMetadata.at(assetId.Hash());
						LoadAsset(helper, assetId, metadata.offset);
					}
				}
				file.close();
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

		InputStreamHelper& AssetManager::GetAssetInputStream(StringID)
		{
			// TODO: Lookup asset registry and find appropriate file / memory stream for read
			istream stream(nullptr);
			InputStreamHelper* helper = new InputStreamHelper(stream);
			return *helper;
		}

		OutputStreamHelper& AssetManager::GetAssetOutputStream(StringID)
		{
			// TODO: Lookup asset registry and find appropriate file / memory stream for write
			ostream stream(nullptr);
			OutputStreamHelper* helper = new OutputStreamHelper(stream);
			return *helper;
		}

		void AssetManager::ReadAssetRegistry(AssetRegistry& registry, InputStreamHelper& helper)
		{
			auto& registryData = registry.Data();
			// Reads asset names
			{
				uint32_t assetsCount;
				helper >> assetsCount;
				string tempName;
				registryData.assets.reserve(assetsCount);
				for (uint32_t i = 0; i < assetsCount; ++i)
				{
					helper >> tempName;
					registryData.assets.push_back(StringID(tempName));
				}
			}
			// Reads asset file names and offset info
			{
				uint32_t filesCount;
				helper >> filesCount;

				uint32_t assetID;
				uint32_t assetOffset;

				registryData.assetFiles.reserve(filesCount);
				for (uint32_t i = 0; i < filesCount; ++i)
				{
					AssetRegistry::FileData fileData;
					helper >> fileData.filename;

					uint32_t assetCount;
					helper >> assetCount;
					fileData.assets.reserve(assetCount);
					for (uint32_t j = 0; j < assetCount; ++i)
					{
						helper >> assetID;
						helper >> assetOffset;
						AssetRegistry::AssetOffsetData offsetData{ StringID(assetID), assetOffset };
						fileData.assets.push_back(offsetData);
					}
					registryData.assetFiles.push_back(fileData);
				}
			}
		}

		void AssetManager::SaveAssetRegistry(AssetRegistry& registry, OutputStreamHelper& helper)
		{
			auto& registryData = registry.Data();
			// Write asset names
			{
				helper << static_cast<uint32_t>(registryData.assets.size());
				for (auto& asset : registryData.assets)
				{
					helper << asset.ToString();
				}
			}
			// Write asset file names and offset info
			{
				helper << static_cast<uint32_t>(registryData.assetFiles.size());
				for (auto& fileData : registryData.assetFiles)
				{
					helper << fileData.filename;
					helper << static_cast<uint32_t>(fileData.assets.size());
					for (auto& offsetData : fileData.assets)
					{
						helper << offsetData.assetName.Hash();
						helper << offsetData.fileOffset;
					}
				}
			}
		}

		void AssetManager::LoadRegistryData(AssetRegistry& registry)
		{
			for (auto& fileData : registry.Data().assetFiles)
			{
				StringID filename(fileData.filename);
				fileMetadata.insert({ filename.Hash(), vector<StringID>()});
				auto& fileAssets = fileMetadata.at(filename.Hash());
				fileAssets.reserve(fileData.assets.size());
				for (auto& assetData : fileData.assets)
				{
					OffsetData data = { assetData.assetName, assetData.fileOffset };
					assetMetadata.insert({ StringID(fileData.filename).Hash(), data });
					fileAssets.push_back(assetData.assetName);
				}
			}
		}

		void AssetManager::OpenFile(ifstream& file, const std::string& filename)
		{
			file.open(filename, ios::binary);
			if (!file.is_open())
			{
				throw GameException("Unable to load file : " + filename);
			}
		}

		void AssetManager::LoadAsset(InputStreamHelper& helper, StringID assetId, std::uint32_t offset)
		{
			helper.Stream().seekg(offset, ios_base::beg);
			uint8_t temp;
			helper >> temp;
			AssetType assetType = static_cast<AssetType>(temp);
			loadedAssets.insert({ assetId.Hash(), Constructors.at(assetType)(assetId) });
		}
	}
}
