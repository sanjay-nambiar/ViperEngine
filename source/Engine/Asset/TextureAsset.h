#pragma once

#include "Asset.h"
#include <cstdint>

namespace Viper
{
	namespace Asset
	{
		class AssetManager;

		class TextureAsset : public Asset
		{
		public:
			TextureAsset(StringID& assetFullName);
			~TextureAsset() = default;

			std::uint32_t Width() const;
			std::uint32_t Height() const;
			std::uint32_t Channels() const;
			const std::uint8_t* Data() const;
		private:
			std::uint32_t width;
			std::uint32_t height;
			std::uint32_t channels;
			std::uint8_t* data;
		};
	}
}
