#pragma once

#include "Graphics/TextureLoader.h"

namespace Viper
{
	namespace Graphics
	{
		class OpenGLTextureLoader final : public TextureLoader
		{
		public:
			OpenGLTextureLoader() = default;
			~OpenGLTextureLoader() = default;

			Graphics::Texture LoadTexture(const std::string& textureFile) override;
		};
	}
}
