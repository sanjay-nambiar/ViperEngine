#pragma once

#include "Asset/TextureAsset.h"
#include "AssetProcessor.h"

namespace AssetPipeline
{
	class TextureProcessor
	{
	public:
		TextureProcessor(AssetProcessor& assetProcessor);
		TextureProcessor(const TextureProcessor&) = delete;
		TextureProcessor(TextureProcessor&&) = delete;

		Viper::Assets::TextureAsset* LoadTexture(const Resource& resource, std::uint32_t index = 0);
	private:
		AssetProcessor& assetProcessor;
	};
}
