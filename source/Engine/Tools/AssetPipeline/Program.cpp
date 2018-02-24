#include "Pch.h"

using namespace std;
using namespace ModelPipeline;
using namespace TexturePipeline;
using namespace Viper;
using namespace Viper::Assets;
using namespace Viper::Memory;

enum class InputAssetType
{
	Invalid,
	Texture,
	Model
};

int PrintUsage(const string& executableName)
{
	cout << "Usage: " << executableName << " -in <infile> <-model | -texture>" << endl;
	return 0;
}

int main(int argc, char* argv[])
{
	// Initialize service locator and asset manager
	MemoryManager allocator;
	ServiceLocator::CreateInstance(allocator);
	auto& serviceLocator = ServiceLocator::GetInstance();
	AssetManager assetManager;
	serviceLocator.Provide(assetManager);

	InputAssetType type = InputAssetType::Invalid;
	string inFilename;
	if (argc < 4)
	{
		return PrintUsage(argv[0]);
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			string param = argv[i];
			if (param == "-model")
			{
				type = InputAssetType::Model;
			}
			else if (param == "-texture")
			{
				type = InputAssetType::Texture;
			}
			else
			{
				if (i + 1 < argc)
				{
					if (param == "-in")
					{
						++i;
						inFilename = argv[i];
					}
				}
				else
				{
					return PrintUsage(argv[0]);
				}
			}
		}
	}

	if (type == InputAssetType::Invalid)
	{
		return PrintUsage(argv[0]);
	}

	try
	{
		auto rawFilename = Utility::GetFilenameWithoutExtension(inFilename);
		if (type == InputAssetType::Model)
		{
			auto modelAsset = ModelProcessor::LoadModel(inFilename, false);
			modelAsset->Save();
			auto modelAsset2 = new ModelAsset(StringID(rawFilename));
			modelAsset2->Load();
			assert(*modelAsset == *modelAsset2);
		}
		else if (type == InputAssetType::Texture)
		{
			auto textureAsset = TextureProcessor::LoadTexture(inFilename);
			textureAsset->Save();
		}
	}
	catch (GameException ex)
	{
		cout << "Error: " << ex.what() << endl;
		return 1;
	}

	serviceLocator.Destroy();
	return 0;
}
