#pragma once

#include "Core/StringID.h"
#include "Core/StreamHelper.h"

namespace Viper
{
	namespace Asset
	{
		enum class AssetType
		{
			Invalid,
			Texture,
			Mesh,
			Material,
			Model,
			Scene,
			Data
		};

		class Asset
		{
		public:
			Asset(const StringID& assetFullName, AssetType type = AssetType::Invalid);
			virtual ~Asset() = default;

			const StringID& AssetFullName() const;
			AssetType Type() const;
			
			void Load();
			void Save();
			void SaveAs(const StringID& assetNewName);

			virtual void Load(InputStreamHelper& inputHelper) = 0;
			virtual void Save(OutputStreamHelper& outputHelper) const = 0;

			StringID assetFullName;
			AssetType type;
		};
	}
}
