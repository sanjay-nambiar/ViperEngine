#pragma once

#include <string>
#include <vector>
#include "Core/StringID.h"
#include "Core/Types.h"

namespace Viper
{
	namespace Assets
	{
		class AssetRegistry
		{
		public:
			struct PackageMeta;

			struct AssetMeta
			{
				StringID assetId;
				StringID packageId;
				uint32_t indexInPackage;
				uint64_t offset;

				AssetMeta(const StringID& assetId);
				bool operator==(const AssetMeta& rhs) const;
			};

			struct PackageMeta
			{
				StringID packageId;
				StringID packageFile;
				std::vector<StringID> assets;

				PackageMeta(const StringID& packageId, const StringID& packageFile, std::uint32_t assetCount = 0);
				bool operator==(const PackageMeta& rhs) const;
			};

			struct RegistryData
			{
				std::unordered_map<StringID, AssetMeta> assets;
				std::unordered_map<StringID, PackageMeta> packages;
				std::string contentDirectory;
				std::string registryFileName;

				bool operator==(const RegistryData& rhs) const;
			};

			RegistryData& Data();
			void Save(bool isDebug = false);
			void Load();
		private:
			void SaveDebug();

			RegistryData data;
		};
	}
}
