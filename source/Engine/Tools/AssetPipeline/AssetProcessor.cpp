#include "Pch.h"

using namespace std;
using namespace Viper;
using namespace Viper::Assets;
using namespace Viper::Memory;

#define LIST_FILE "list.temp"
#define TEMP_FILE ".tempFile"

namespace AssetPipeline
{
	AssetProcessor::AssetProcessor() :
		isPackagedMode(false)
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

	void AssetProcessor::GenerateAssetList(const string& assetsDir, const string& contentDir, bool isPackaged)
	{
		cout << "Generating asset list : " << LIST_FILE << " ... ";
		resourcesDirectory = assetsDir;
		string command = "python AssetRegistryHelper.py registry " + assetsDir + " " + contentDir;
		if (isPackaged)
		{
			command += " --packaged";
		}
		uint32_t exitCode = system(command.c_str());
		if (exitCode)
		{
			throw GameException("Error running list file generator script.");
		}
		cout << "DONE" << endl;
		isPackagedMode = isPackaged;
	}

	void AssetProcessor::LoadAssetList()
	{
		ifstream file;
		file.open(LIST_FILE);
		if (!file.is_open())
		{
			throw GameException("Unable to read assets list file: " + string(LIST_FILE));
		}

		cout << "Parsing list file..." << endl;
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
			cout << "Reading resource details..." << endl;
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
					else if (string(resourceInfoIt->key) == string("packageFile"))
					{
						assert(resourceInfoIt->value.getTag() == JSON_STRING);
						resource.packageFile = string(resourceInfoIt->value.toString());
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
				cout << "Resource detail read for : " << resource.resourceFile << endl;
			}
			cout << "Resource details read." << endl;
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
		cout << "Parsing complete." << endl;
		CreateRegistry();
	}

	void AssetProcessor::RegisterOffset(Asset& asset, const StringID& packageId, const StringID& packageFile)
	{
		auto& registryData = assetManager->Registry().Data();
		auto& assetId = asset.AssetId();
		if (registryData.assets.find(assetId) == registryData.assets.end())
		{
			assetPackageMap.insert({ assetId, { packageId, packageFile } });
			registryData.assets.insert({ assetId, AssetRegistry::AssetMeta(assetId) });
			auto& assetMeta = registryData.assets.at(assetId);
			StringID tempPackageId(tempDirectory + to_string(assetId.Hash()));
			registryData.packages.insert({ tempPackageId, AssetRegistry::PackageMeta(tempPackageId, tempPackageId) });
			tempPackageIds.push_back(tempPackageId);
			auto& packageMeta = registryData.packages.at(tempPackageId);
			packageMeta.assets.push_back(assetId);
			assetMeta.packageId = tempPackageId;
			loadedAssets.insert({ assetId, &asset });
		}
	}

	void AssetProcessor::Cleanup(const string& contentDir)
	{
		cout << "Cleaning up temporary files... ";
		string command = "python AssetRegistryHelper.py cleanup " + contentDir;
		uint32_t exitCode = system(command.c_str());
		if (exitCode)
		{
			throw GameException("Error running cleanup script.");
		}
		cout << "DONE" << endl;
	}

	void AssetProcessor::SaveAssets(bool isDebug)
	{
		auto saveResources = [&](vector<Resource> resources, function<Asset*(const Resource&)> loadFunction)
		{
			for (auto& resource : resources)
			{
				cout << "Loading asset data from resource. Resource file : " << resource.resourceFile << ", Package : " << resource.packageName << endl;
				Asset* asset = loadFunction(resource);
				cout << "Load complete." << endl;
				if (asset != nullptr)
				{
					cout << "Saving asset : " << asset->AssetId().ToString() << " ... ";
					asset->Save();
					cout << "DONE" << endl;
				}
			}
		};
		cout << "Load and save textures..." << endl;
		saveResources(textures, [&](const Resource& resource) {
			return textureProcessor->LoadTexture(resource);
		});
		cout << "Textures saved." << endl;
		cout << "Load and save models..." << endl;
		saveResources(models, [&](const Resource& resource) {
			return modelProcessor->LoadModel(resource);
		});
		cout << "Models saved." << endl;

		auto& registryData = assetManager->Registry().Data();
		cout << "Updating registry data and saving assets to correct files..." << endl;
		for (auto& assetMetaEntry : registryData.assets)
		{
			auto& assetMeta = assetMetaEntry.second;
			cout << "Finalize registry data for : " << assetMeta.assetId.ToString() << " ... ";
			ifstream src;
			const string srcFile = registryData.contentDirectory + assetMeta.packageId.ToString() + AssetManager::DefaultAssetExtension;
			src.open(srcFile, ios::binary);
			if (!src.is_open())
			{
				throw GameException("Unable to open file : " + srcFile);
			}

			ofstream dest;
			auto& packageInfo = assetPackageMap.at(assetMeta.assetId);
			auto& packageId = packageInfo.first;
			auto& packageFile = packageInfo.second;
			const string destFile = registryData.contentDirectory + packageFile.ToString() + AssetManager::DefaultAssetExtension;
			dest.open(destFile, ios::binary | ios::ate | ios::app);
			if (!dest.is_open())
			{
				src.close();
				throw GameException("Unable to open file : " + destFile);
			}

			assetMeta.packageId = packageId;
			assetMeta.offset = static_cast<uint64_t>(dest.tellp());
			if (registryData.packages.find(packageId) == registryData.packages.end())
			{
				registryData.packages.insert({ packageId, AssetRegistry::PackageMeta(packageId, packageFile) });
			}
			auto& packageMeta = registryData.packages.at(packageId);
			assetMeta.indexInPackage = static_cast<uint32_t>(packageMeta.assets.size());
			packageMeta.assets.push_back(assetMeta.assetId);
			dest << src.rdbuf();
			cout << "DONE" << endl;
		}

		for (auto& tempPackageId : tempPackageIds)
		{
			registryData.packages.erase(tempPackageId);
		}
		cout << "Registry data updated. Assets finalized." << endl;
		cout << "Saving registry data... ";
		assetManager->Registry().Save(isDebug);
		cout << "DONE" << endl;
	}

	void AssetProcessor::CreateRegistry()
	{
		cout << "Creating base registry object...";
		auto& registryData = assetManager->Registry().Data();
		registryData.contentDirectory = contentDirectory;
		registryData.registryFileName = AssetManager::DefaultRegistryFile;
		registryData.packages.reserve(textures.size() + models.size());

		auto addResourceEntries = [&](vector<Resource> resources)
		{
			for (auto& resource : resources)
			{
				StringID packageId(resource.packageName);
				StringID packageFile(resource.packageFile);
				registryData.packages.insert({ packageId, AssetRegistry::PackageMeta(packageId, packageFile) });
			}
		};
		addResourceEntries(textures);
		addResourceEntries(models);
		cout << "DONE" << endl;
	}

	void AssetProcessor::GetResource(Resource& resource, const string& path, const string& base)
	{
		string command = "python AssetRegistryHelper.py relative \"" + path + "\" \"" + base + "\" \"" + resourcesDirectory +
			"\" \"" + contentDirectory;
		if (isPackagedMode)
		{
			command += " --packaged";
		}
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
			else if (string(resourceInfoIt->key) == string("packageFile"))
			{
				assert(resourceInfoIt->value.getTag() == JSON_STRING);
				resource.packageFile = string(resourceInfoIt->value.toString());
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

	void AssetProcessor::ValidateRegistryAndAssets()
	{
		cout << "Validating registry and assets..." << endl;
		// Create new assetmanager for validation purpose
		auto& assetManagerValidator = *(new (allocator.Allocate<AssetManager>()) AssetManager(allocator));
		serviceLocator->Provide(assetManagerValidator);
		auto& validatorRegistry = assetManagerValidator.Registry();
		auto& validatorRegistryData = validatorRegistry.Data();
		validatorRegistryData.contentDirectory = AssetManager::DefaultContentDirectory;
		validatorRegistryData.registryFileName = AssetManager::DefaultRegistryFile;
		
		// validate
		validatorRegistry.Load();
		auto& registryData = assetManager->Registry().Data();
		cout << "Registry data validated." << endl;
		bool validationSuccess = (registryData == validatorRegistryData);
		if (validationSuccess)
		{
			cout << "Loading all assets..." << endl;
			assetManagerValidator.LoadAll();
			for (auto& entry : assetManagerValidator.LoadedAssets())
			{
				auto& assetId = entry.first;
				auto& asset = entry.second;
				assert(loadedAssets.find(assetId) != loadedAssets.end());
				auto& oldAsset = loadedAssets.at(assetId);
				cout << "Validating asset: " << assetId.ToString() << " ... ";
				bool assetValid = (*asset == *oldAsset);
				validationSuccess &= assetValid;
				if (assetValid)
				{
					cout << "DONE" << endl;
				}
				else
				{
					cout << "ERROR" << endl;
				}
			}

			if (validationSuccess)
			{
				cout << "Asset data validation successful." << endl;
			}
			else
			{
				cout << "Asset data validation failed." << endl;
			}
		}
		else
		{
			cout << "Asset registry data mismatch during validation." << endl;
		}
	}
}
