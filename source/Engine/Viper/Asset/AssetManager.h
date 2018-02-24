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

namespace Viper
{
	class StringID;

	namespace Assets
	{
		class AssetManager final : public Service
		{
		public:
			AssetManager();
			~AssetManager() = default;

			void Initialize(const std::string& assetRegistryFile);
			void Shutdown();

			Asset* LoadSynchronous(const StringID& assetFullName);
			void UnloadSynchronous(const StringID& assetFullName);

			void LoadAll();
			void UnloadAll();
		private:
			struct OffsetData
			{
				StringID resourceFile;
				std::uint32_t offset;
			};

			InputStreamHelper& GetAssetInputHelper(StringID assetFullname);
			OutputStreamHelper& GetAssetOutputHelper(StringID assetFullname);

			static void ReadAssetRegistry(AssetRegistry& registry, InputStreamHelper& helper);
			static void SaveAssetRegistry(AssetRegistry& registry, OutputStreamHelper& helper);

			void LoadRegistryData(AssetRegistry& registry);
			void OpenFile(std::ifstream& file, const std::string& filename);
			Asset* LoadAsset(InputStreamHelper& helper, StringID assetId, std::uint32_t offset);

			std::unordered_map<std::uint32_t, OffsetData> assetMetadata;
			std::unordered_map<std::uint32_t, std::vector<StringID>> fileMetadata;
			std::unordered_map<std::uint32_t, Asset*> loadedAssets;

			static std::unordered_map<AssetType, AssetConstructor> Constructors;

			friend class Asset;
		};
	}
}
