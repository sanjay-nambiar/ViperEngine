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
				std::vector<StringID> assets;

				PackageMeta(const StringID& packageId, std::uint32_t assetCount = 0);
			};

			struct RegistryData
			{
				std::unordered_map<StringID, AssetMeta> assets;
				std::unordered_map<StringID, PackageMeta> packages;
				std::string contentDirectory;

				static const std::string AssetExtension;
			};

			RegistryData& Data();
		private:
			RegistryData data;
		};
	}
}
