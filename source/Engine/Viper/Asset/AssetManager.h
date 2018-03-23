#pragma once

#include <fstream>
#include <unordered_set>
#include <vector>
#include "Asset/Asset.h"
#include "Asset/AssetRegistry.h"
#include "Asset/AssetType.h"
#include "Core/Core.h"
#include "Core/Service.h"
#include "Core/StreamHelper.h"
#include "Core/Types.h"
#include "Memory/MemoryAllocator.h"

namespace Viper
{
	class StringID;

	namespace Assets
	{
		class AssetManager final : public Service
		{
		public:
			AssetManager(Memory::MemoryAllocator& allocator);
			~AssetManager() = default;

			void Initialize(const std::string& assetRegistryFile);
			void Shutdown();

			Asset* LoadSynchronous(const StringID& assetFullName);
			void UnloadSynchronous(const StringID& assetFullName);
			void SaveSynchronous(const Asset& asset);

			void LoadAll();
			void UnloadAll();

			AssetRegistry& Registry();
		private:
			void ReadAssetRegistry(InputStreamHelper& helper);
			void SaveAssetRegistry(OutputStreamHelper& helper);

			void OpenInputFile(std::ifstream& file, const std::string& filename);
			void OpenOutputFile(std::ofstream& file, const std::string& filename);
			Asset* LoadAsset(InputStreamHelper& helper, const AssetRegistry::AssetMeta& assetMeta);

			Memory::MemoryAllocator& allocator;
			AssetRegistry registry;
			std::unordered_map<StringID, Asset*> loadedAssets;

			static std::unordered_map<AssetType, AssetConstructor>& Constructors();

			friend class Asset;
		};
	}
}
