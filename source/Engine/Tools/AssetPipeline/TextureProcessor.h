#pragma once

#include "Asset/TextureAsset.h"

namespace TexturePipeline
{
	class TextureProcessor
	{
	public:
		TextureProcessor() = delete;

		static Viper::Assets::TextureAsset* LoadTexture(const std::string& filename);
	};
}
