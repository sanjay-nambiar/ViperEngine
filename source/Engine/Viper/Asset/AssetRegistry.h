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
				uint32_t offset;

				AssetMeta(const StringID& assetId);
			};

			struct PackageMeta
			{
				StringID packageId;
				StringID packageFile;
				std::vector<StringID> assets;

				PackageMeta(const StringID& packageId, const StringID& packageFile, std::uint32_t assetCount = 0);
			};

			struct RegistryData
			{
				std::unordered_map<StringID, AssetMeta> assets;
				std::unordered_map<StringID, PackageMeta> packages;
				std::string contentDirectory;
			};

			RegistryData& Data();
			void Load(const std::string& contentDirectory);
			void Save(bool isDebug = false);

			static const std::string AssetExtension;
			static const std::string AssetRegistryFile;
		private:
			void SaveDebug();

			RegistryData data;
		};
	}
}
