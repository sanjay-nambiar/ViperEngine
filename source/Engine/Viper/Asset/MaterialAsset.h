#pragma once

#include "Asset.h"

namespace Viper
{
	namespace Asset
	{
		class TextureAsset;

		class MaterialAsset : public Asset
		{
		public:
			MaterialAsset(const StringID& assetFullName);
			~MaterialAsset() = default;

			const TextureAsset* NormalMap() const;
		protected:
			const TextureAsset* normalMap;
		};
	}
}
