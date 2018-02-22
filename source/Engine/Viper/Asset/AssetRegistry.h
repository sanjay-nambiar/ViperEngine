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
			struct AssetOffsetData
			{
				StringID assetName;
				uint32_t fileOffset;
			};

			struct FileData
			{
				std::string filename;
				std::vector<AssetOffsetData> assets;
			};

			struct RegistryFileData
			{
				std::vector<StringID> assets;
				std::vector<FileData> assetFiles;
			};

			RegistryFileData& Data();
		private:
			RegistryFileData data;
		};
	}
}
