#pragma once

#include "Service/TextureLoader.h"

namespace Viper
{
	namespace Renderer
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
