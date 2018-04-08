#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace Viper { namespace Memory { class MemoryManager; } };
namespace Viper { namespace Assets { class AssetManager; } };
namespace Viper { class ServiceLocator; };

namespace AssetPipeline
{
	class ModelProcessor;
	class MeshProcessor;
	class MaterialProcessor;
	class TextureProcessor;

	struct Resource
	{
		std::string packageName;
		std::string packageFile;
		std::string packageDir;
		std::string resourceDir;
		std::string resourceFile;
	};

	class AssetProcessor
	{
	public:
		AssetProcessor();
		~AssetProcessor();
		void GenerateAssetList(const std::string& assetsDir, const std::string& contentDir, bool isPackaged);
		void LoadAssetList();
		void RegisterOffset(Viper::Assets::Asset& asset, const Viper::StringID& packageId, const Viper::StringID& packageFile);
		void Cleanup(const std::string& contentDir);
		void SaveAssets(bool isDebug);
		void GetResource(Resource& resource, const std::string& relativePath, const std::string& relativeTo);
		Viper::Assets::Asset* GetLoadedAsset(const Viper::StringID& assetId);
	private:
		void CreateRegistry();

		Viper::Memory::MemoryManager allocator;
		Viper::ServiceLocator* serviceLocator;
		Viper::Assets::AssetManager* assetManager;
		std::string resourcesDirectory;
		std::string contentDirectory;
		std::string contentDirectoryPath;
		std::string tempDirectory;
		std::vector<Resource> models;
		std::vector<Resource> textures;
		std::unordered_map<Viper::StringID, std::pair<Viper::StringID, Viper::StringID>> assetPackageMap;
		std::vector<Viper::StringID> tempPackageIds;
		std::unordered_map<Viper::StringID, Viper::Assets::Asset*> loadedAssets;

		ModelProcessor* modelProcessor;
		MeshProcessor* meshProcessor;
		MaterialProcessor* materialProcessor;
		TextureProcessor* textureProcessor;
		bool isPackagedMode;
	};
}
