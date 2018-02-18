#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Assets
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

			virtual bool operator==(const MaterialAsset& rhs) const = 0;
			virtual bool operator!=(const MaterialAsset& rhs) const = 0;
		protected:
			virtual const MaterialData& BaseData() const = 0;

			static TextureAsset* LoadTextureHelper(InputStreamHelper& helper);
			static void SaveTextureHelper(TextureAsset* textureAsset, OutputStreamHelper& helper);
		};

#define IS_TEXTURE_EQUAL(texture1, texture2)	((texture1 == texture2) ||      \
		(texture1 != nullptr && texture2 != nullptr && *texture1 == *texture2))

	}
}
