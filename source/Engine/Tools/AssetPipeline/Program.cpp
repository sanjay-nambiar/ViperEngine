#include "Pch.h"

using namespace std;
using namespace ModelPipeline;
using namespace TexturePipeline;
using namespace Viper;
using namespace Viper::Assets;
using namespace Viper::Memory;

#define LIST_FILE "list.temp"

struct AssetDetails
{
	string name;
	string package;
	string sourceFile;
};

int PrintUsage(const string& executableName)
{
	cout << "Usage: " << executableName << " <assets source directory> <game content directory>" << endl;
	return 0;
}

void LoadAssetList(const string& assetsList, vector<AssetDetails>& assetDetails)
{
	ifstream file;
	file.open(assetsList);
	if (!file.is_open())
	{
		throw GameException("Unable to read assets list file: " + assetsList);
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

	uint32_t assetCount = 0;
	for (auto registryIt : jsonObject)
	{
		if (string("resources") == registryIt->key)
		{
			assert(registryIt->value.getTag() == JSON_ARRAY);
			auto& resources = registryIt->value;
			for (auto resourcesIt : resources)
			{
				auto& assetInfo = resourcesIt->value;
				AssetDetails detail;
				for (auto assetInfoIt : assetInfo)
				{
					assert(assetInfoIt->value.getTag() == JSON_STRING);
					if (string(assetInfoIt->key) == string("name"))
					{
						detail.name = string(assetInfoIt->value.toString());
					}
					else if (string(assetInfoIt->key) == string("package"))
					{
						detail.package = string(assetInfoIt->value.toString());
					}
					else if (string(assetInfoIt->key) == string("sourceFile"))
					{
						detail.sourceFile = string(assetInfoIt->value.toString());
					}
					else
					{
						throw GameException("Unknown data in asset registry.");
					}
				}
				assetDetails.push_back(detail);
			}
		}
		else if (string("count") == registryIt->key)
		{
			assert(registryIt->value.getTag() == JSON_NUMBER);
			assetCount = static_cast<uint32_t>(registryIt->value.toNumber());
			assetDetails.reserve(assetCount);
		}
		else
		{
			throw GameException("Unknown data in registry json");
		}
	}
	assert(assetCount == assetDetails.size());
}

int main(int argc, char* argv[])
{
	// Initialize service locator and asset manager
	MemoryManager allocator;
	ServiceLocator::CreateInstance(allocator);
	auto& serviceLocator = ServiceLocator::GetInstance();
	AssetManager assetManager;
	serviceLocator.Provide(assetManager);

	if (argc != 3)
	{
		return PrintUsage(argv[0]);
	}

	try
	{
		const std::string assetsDir(argv[1]);
		const std::string contentDir(argv[2]);
		string command = "python GenerateAssetRegistry.py " + assetsDir + " " + contentDir;
		uint32_t exitCode = system(command.c_str());
		if (exitCode)
		{
			throw GameException("Error running list file generator script.");
		}
		vector<AssetDetails> assetDetails;
		LoadAssetList(LIST_FILE, assetDetails);
	}
	catch (GameException ex)
	{
		cout << "Error: " << ex.what() << endl;
		return 1;
	}
	serviceLocator.Destroy();
	return 0;
}
