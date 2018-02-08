#include "Viper.h"
#include "TextureAsset.h"
#include "stb_image.h"

using namespace std;

namespace Viper
{
	namespace Asset
	{
		TextureAsset::TextureAsset(const StringID& assetFullName) :
			Asset(assetFullName), image({ nullptr }), width(0), height(0), channels(0)
		{
			const auto& filePath = assetFullName.ToString();
			int w, h, ch;
			if (stbi_is_hdr(filePath.c_str()) == 0)
			{
				image.data = static_cast<uint8_t*>(stbi_load(filePath.c_str(), &w, &h, &ch, 0));
			}
			else
			{
				image.hdrData = stbi_loadf(filePath.c_str(), &w, &h, &ch, 0);
			}
			width = static_cast<uint32_t>(w);
			height = static_cast<uint32_t>(h);
			channels = static_cast<uint32_t>(ch);
		}

		uint32_t TextureAsset::Width() const
		{
			return width;
		}

		uint32_t TextureAsset::Height() const
		{
			return height;
		}

		uint32_t TextureAsset::Channels() const
		{
			return channels;
		}

		const uint8_t* TextureAsset::Data() const
		{
			return image.data;
		}

		const float32_t* TextureAsset::HdrData() const
		{
			return image.hdrData;
		}
	}
}
