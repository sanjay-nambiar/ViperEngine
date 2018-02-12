#include "Pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace Viper;
using namespace Viper::Asset;

namespace TexturePipeline
{
	TextureAsset* TextureProcessor::LoadTexture(const string& filename)
	{
		auto textureAsset = new TextureAsset(StringID(filename));
		auto& textureData = textureAsset->Data();

		int w, h, ch;
		if (stbi_is_hdr(filename.c_str()) == 0)
		{
			textureData.image.data = static_cast<uint8_t*>(stbi_load(filename.c_str(), &w, &h, &ch, 0));
		}
		else
		{
			textureData.isHdr = true;
			textureData.image.hdrData = stbi_loadf(filename.c_str(), &w, &h, &ch, 0);
		}
		textureData.width = static_cast<uint32_t>(w);
		textureData.height = static_cast<uint32_t>(h);
		textureData.channels = static_cast<uint32_t>(ch);
		return textureAsset;
	}
}
