#include "Pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace Viper;
using namespace Viper::Assets;

namespace AssetPipeline
{
	TextureProcessor::TextureProcessor(AssetProcessor& assetProcessor) :
		assetProcessor(assetProcessor)
	{
	}

	TextureAsset* TextureProcessor::LoadTexture(const Resource& resource, uint32_t index)
	{
		StringID assetId = StringID(resource.packageName + ":texture." + to_string(index));
		Asset* asset = assetProcessor.GetLoadedAsset(assetId);
		if (asset != nullptr)
		{
			return static_cast<TextureAsset*>(asset);
		}

		auto textureAsset = new TextureAsset(assetId);
		assetProcessor.RegisterOffset(*textureAsset, resource.packageName);
		auto& textureData = textureAsset->Data();

		int w, h, ch;
		if (stbi_is_hdr(resource.resourceFile.c_str()) == 0)
		{
			textureData.image.data = static_cast<uint8_t*>(stbi_load(resource.resourceFile.c_str(), &w, &h, &ch, 0));
		}
		else
		{
			textureData.isHdr = true;
			textureData.image.hdrData = stbi_loadf(resource.resourceFile.c_str(), &w, &h, &ch, 0);
		}
		textureData.width = static_cast<uint32_t>(w);
		textureData.height = static_cast<uint32_t>(h);
		textureData.channels = static_cast<uint32_t>(ch);
		return textureAsset;
	}
}
