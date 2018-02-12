#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		class TextureAsset;

		struct MaterialData
		{
			bool isPbr;
			TextureAsset* normalMap;

			MaterialData();
			virtual ~MaterialData() = default;
		};

		class MaterialAsset : public Asset
		{
		public:
			MaterialAsset(const StringID& assetFullName);
			~MaterialAsset() = default;

			bool IsPbr() const;
		protected:
			virtual const MaterialData& BaseData() const = 0;

			static TextureAsset* LoadTextureHelper(InputStreamHelper& helper);
			static void SaveTextureHelper(TextureAsset* textureAsset, OutputStreamHelper& helper);
		};
	}
}
