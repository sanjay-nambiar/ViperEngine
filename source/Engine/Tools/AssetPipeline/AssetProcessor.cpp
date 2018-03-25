#include "Pch.h"

using namespace std;
using namespace Viper;
using namespace Viper::Assets;
using namespace Viper::Memory;

#define LIST_FILE "list.temp"
#define TEMP_FILE ".tempFile"

namespace AssetPipeline
{
	AssetProcessor::AssetProcessor()
	{
		ServiceLocator::CreateInstance(allocator);
		serviceLocator = &ServiceLocator::GetInstance();
		assetManager = new (allocator.Allocate<AssetManager>()) AssetManager(allocator);
		serviceLocator->Provide(*assetManager);

		meshProcessor = new (allocator.Allocate<MeshProcessor>()) MeshProcessor(*this);
		textureProcessor = new (allocator.Allocate<TextureProcessor>()) TextureProcessor(*this);
		materialProcessor = new (allocator.Allocate<MaterialProcessor>()) MaterialProcessor(*this, *textureProcessor);
		modelProcessor = new (allocator.Allocate<ModelProcessor>()) ModelProcessor(*this, *meshProcessor, *materialProcessor);
	}

	AssetProcessor::~AssetProcessor()
	{
		serviceLocator->Destroy();
		assetManager->~AssetManager();
		allocator.Free(assetManager);
		allocator.Free(modelProcessor);
		allocator.Free(materialProcessor);
		allocator.Free(textureProcessor);
	}

	void AssetProcessor::LoadAssetList(const string& assetsDir, const string& contentDir)
	{
		resourcesDirectory = assetsDir;
		string command = "python AssetRegistryHelper.py registry " + assetsDir + " " + contentDir;
		uint32_t exitCode = system(command.c_str());
		if (exitCode)
		{
			throw GameException("Error running list file generator script.");
		}

		ifstream file;
		file.open(LIST_FILE);
		if (!file.is_open())
		{
			throw GameException("Unable to read assets list file: " + string(LIST_FILE));
		}

		string assetsJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		char *endptr;
		JsonValue jsonObject;
		JsonAllocator jsonAllocator;
		int status = jsonParse(const_cast<char*>(assetsJson.c_str()), &endptr, &jsonObject, jsonAllocator);
		if (status != JSON_OK)
		{
			throw GameException(string(jsonStrError(status)) + " " + to_string(static_cast<size_t>(endptr - assetsJson.c_str())));
		}

		auto readResourceDetails = [&](JsonNode* registryIt, vector<Resource>& resourceDetails)
		{
			assert(registryIt->value.getTag() == JSON_ARRAY);
			auto& resources = registryIt->value;
			for (auto resourcesIt : resources)
			{
				auto& resourceInfo = resourcesIt->value;
				Resource resource;
				for (auto resourceInfoIt : resourceInfo)
				{
					if (string(resourceInfoIt->key) == string("packageName"))
					{
						assert(resourceInfoIt->value.getTag() == JSON_STRING);
						resource.packageName = string(resourceInfoIt->value.toString());
					}
					else if (string(resourceInfoIt->key) == string("packageDir"))
					{
						assert(resourceInfoIt->value.getTag() == JSON_STRING);
						resource.packageDir = string(resourceInfoIt->value.toString());
					}
					else if (string(resourceInfoIt->key) == string("resourceDir"))
					{
						assert(resourceInfoIt->value.getTag() == JSON_STRING);
						resource.resourceDir = string(resourceInfoIt->value.toString());
					}
					else if (string(resourceInfoIt->key) == string("resourceFile"))
					{
						assert(resourceInfoIt->value.getTag() == JSON_STRING);
						resource.resourceFile = string(resourceInfoIt->value.toString());
					}
				}
				resourceDetails.push_back(resource);
			}
		};

		uint32_t resourceCount = 0;
		for (auto registryIt : jsonObject)
		{
			if (string("textures") == registryIt->key)
			{
				readResourceDetails(registryIt, textures);
			}
			if (string("models") == registryIt->key)
			{
				readResourceDetails(registryIt, models);
			}
			else if (string("count") == registryIt->key)
			{
				assert(registryIt->value.getTag() == JSON_NUMBER);
				resourceCount = static_cast<uint32_t>(registryIt->value.toNumber());
			}
			else if (string("contentDirectory") == registryIt->key)
			{
				assert(registryIt->value.getTag() == JSON_STRING);
				contentDirectory = registryIt->value.toString();
			}
			else if (string("contentDirectoryPath") == registryIt->key)
			{
				assert(registryIt->value.getTag() == JSON_STRING);
				contentDirectoryPath = registryIt->value.toString();
			}
			else if (string("tempDirectory") == registryIt->key)
			{
				assert(registryIt->value.getTag() == JSON_STRING);
				tempDirectory = registryIt->value.toString();
			}
		}
		assert(resourceCount == textures.size() + models.size());
		CreateRegistry();
	}

	void AssetProcessor::RegisterOffset(Asset& asset, const StringID& packageId)
	{
		auto& registryData = assetManager->Registry().Data();
		auto& assetId = asset.AssetId();
		if (registryData.assets.find(assetId) == registryData.assets.end())
		{
			assetPackageMap.insert({ assetId, packageId });
			registryData.assets.insert({ assetId, AssetRegistry::AssetMeta(assetId) });
			auto& assetMeta = registryData.assets.at(assetId);

			StringID tempPackageId(tempDirectory + to_string(assetId.Hash()));
			registryData.packages.insert({ tempPackageId, AssetRegistry::PackageMeta(tempPackageId) });
			tempPackageIds.push_back(tempPackageId);
			auto& packageMeta = registryData.packages.at(tempPackageId);
			packageMeta.assets.push_back(assetId);
			assetMeta.packageId = tempPackageId;
			loadedAssets.insert({ assetId, &asset });
		}
	}

	void AssetProcessor::SaveAssets()
	{
		auto saveResources = [&](vector<Resource> resources, function<Asset*(const Resource&)> loadFunction)
		{
			for (auto& resource : resources)
			{
				Asset* asset = loadFunction(resource);
				if (asset != nullptr)
				{
					asset->Save();
				}
			}
		};
		saveResources(textures, [&](const Resource& resource) {
			return textureProcessor->LoadTexture(resource);
		});
		saveResources(models, [&](const Resource& resource) {
			return modelProcessor->LoadModel(resource);
		});

		auto& registryData = assetManager->Registry().Data();
		for (auto& assetMetaEntry : registryData.assets)
		{
			auto& assetMeta = assetMetaEntry.second;

			ifstream src;
			const string srcFile = registryData.contentDirectory + assetMeta.packageId.ToString() + AssetRegistry::AssetExtension;
			src.open(srcFile, ios::binary);
			if (!src.is_open())
			{
				throw GameException("Unable to open file : " + srcFile);
			}

			ofstream dest;
			auto& packageId = assetPackageMap.at(assetMeta.assetId);
			const string destFile = registryData.contentDirectory + packageId.ToString() + AssetRegistry::AssetExtension;
			dest.open(destFile, ios::binary | ios::ate | ios::app);
			if (!dest.is_open())
			{
				src.close();
				throw GameException("Unable to open file : " + destFile);
			}

			assetMeta.packageId = packageId;
			assetMeta.offset = static_cast<uint32_t>(dest.tellp());
			if (registryData.packages.find(packageId) == registryData.packages.end())
			{
				registryData.packages.insert({ packageId, AssetRegistry::PackageMeta(packageId) });
			}
			auto& packageMeta = registryData.packages.at(packageId);
			assetMeta.indexInPackage = static_cast<uint32_t>(packageMeta.assets.size());
			packageMeta.assets.push_back(assetMeta.assetId);
			dest << src.rdbuf();
		}

		for (auto& tempPackageId : tempPackageIds)
		{
			registryData.packages.erase(tempPackageId);
		}
		assetManager->Registry().Save();
	}

	void AssetProcessor::CreateRegistry()
	{
		auto& registryData = assetManager->Registry().Data();
		registryData.contentDirectory = contentDirectory;
		registryData.packages.reserve(textures.size() + models.size());
		
		auto addResourceEntries = [&](vector<Resource> resources)
		{
			for (auto& resource : resources)
			{
				StringID packageId(resource.packageName);
				registryData.packages.insert({ packageId, AssetRegistry::PackageMeta(packageId) });
			}
		};
		addResourceEntries(textures);
		addResourceEntries(models);
	}

	void AssetProcessor::GetResource(Resource& resource, const string& path, const string& base)
	{
		string command = "python AssetRegistryHelper.py relative \"" + path + "\" \"" + base + "\" \"" + resourcesDirectory +
			"\" \"" + contentDirectory;
		uint32_t exitCode = system(command.c_str());
		if (exitCode)
		{
			throw GameException("Error running path helper script.");
		}

		ifstream file;
		file.open(TEMP_FILE);
		if (!file.is_open())
		{
			throw GameException("Unable to read temp file: " + string(TEMP_FILE));
		}

		string tempJson((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		char *endptr;
		JsonValue jsonObject;
		JsonAllocator jsonAllocator;
		int status = jsonParse(const_cast<char*>(tempJson.c_str()), &endptr, &jsonObject, jsonAllocator);
		if (status != JSON_OK)
		{
			throw GameException(string(jsonStrError(status)) + " " + to_string(static_cast<size_t>(endptr - tempJson.c_str())));
		}

		for (auto resourceInfoIt : jsonObject)
		{
			if (string(resourceInfoIt->key) == string("packageName"))
			{
				assert(resourceInfoIt->value.getTag() == JSON_STRING);
				resource.packageName = string(resourceInfoIt->value.toString());
			}
			else if (string(resourceInfoIt->key) == string("packageDir"))
			{
				assert(resourceInfoIt->value.getTag() == JSON_STRING);
				resource.packageDir = string(resourceInfoIt->value.toString());
			}
			else if (string(resourceInfoIt->key) == string("resourceDir"))
			{
				assert(resourceInfoIt->value.getTag() == JSON_STRING);
				resource.resourceDir = string(resourceInfoIt->value.toString());
			}
			else if (string(resourceInfoIt->key) == string("resourceFile"))
			{
				assert(resourceInfoIt->value.getTag() == JSON_STRING);
				resource.resourceFile = string(resourceInfoIt->value.toString());
			}
		}
	}

	Asset* AssetProcessor::GetLoadedAsset(const StringID& assetId)
	{
		Asset* asset = nullptr;
		if (loadedAssets.find(assetId) != loadedAssets.end())
		{
			asset = loadedAssets.at(assetId);
		}
		return asset;
	}
}
