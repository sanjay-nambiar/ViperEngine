#include "Pch.h"
#include "TextureAsset.h"

using namespace std;

namespace Viper
{
	namespace Asset
	{
		TextureAsset::TextureAsset(StringID& assetFullName) :
			Asset(assetFullName), data(nullptr), width(0), height(0), channels(0)
		{
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
			return data;
		}
	}
}
