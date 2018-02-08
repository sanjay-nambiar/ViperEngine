#pragma once

#include <unordered_set>
#include <vector>
#include "Asset/Asset.h"
#include "Core/Core.h"

namespace Viper
{
	class StringID;

	namespace Asset
	{
		class AssetManager
		{
		public:
			void LoadSynchronous(const StringID& assetFullName);
			void UnloadSynchronous(const StringID& assetFullName);
			void UnloadAll();
			std::vector<Asset> Search(const StringID& assetSearchPath);
		private:
			std::vector<Asset> loadedAssets;
			std::unordered_set<Asset> loadedAssetsLookup;
		};
	}
}
