#pragma once

#include "Asset.h"
#include "Core/Types.h"

namespace Viper
{
	namespace Asset
	{
		class AssetManager;

		class TextureAsset : public Asset
		{
		public:
			TextureAsset(const StringID& assetFullName);
			~TextureAsset() = default;

			std::uint32_t Width() const;
			std::uint32_t Height() const;
			std::uint32_t Channels() const;
			const std::uint8_t* Data() const;
			const float* HdrData() const;
		private:
			std::uint32_t width;
			std::uint32_t height;
			std::uint32_t channels;
			union
			{
				std::uint8_t* data;
				float* hdrData;
			} image;
		};
	}
}
