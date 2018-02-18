#pragma once

#include <iostream>
#include "Core/StringID.h"
#include "Core/StreamHelper.h"

namespace Viper
{
	namespace Assets
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

		class AssetManager;

		class Asset
		{
		public:
			Asset(const StringID& assetFullName, AssetType type = AssetType::Invalid);
			virtual ~Asset() = default;

			const StringID& AssetFullName() const;
			AssetType Type() const;

			void Load();
			void Save();

			StringID assetFullName;
			AssetType type;
		protected:
			virtual void LoadFrom(InputStreamHelper& inputHelper) = 0;
			virtual void SaveTo(OutputStreamHelper& outputHelper) const = 0;
		};
	}
}
