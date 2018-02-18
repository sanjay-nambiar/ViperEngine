#pragma once

#include <unordered_set>
#include <vector>
#include "Asset/Asset.h"
#include "Core/Core.h"
#include "Core/Service.h"
#include "Core/StreamHelper.h"

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
			void LoadSynchronous(const StringID& assetFullName);
			void UnloadSynchronous(const StringID& assetFullName);
			void UnloadAll();
			std::vector<Asset> Search(const StringID& assetSearchPath);
		private:
			InputStreamHelper& GetAssetInputStream(StringID assetFullname);
			OutputStreamHelper& GetAssetOutputStream(StringID assetFullname);

			std::vector<Asset> loadedAssets;
			std::unordered_set<uint32_t> loadedAssetsLookup;

			friend class Asset;
		};
	}
}
