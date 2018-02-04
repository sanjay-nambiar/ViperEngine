#pragma once

#include "Graphics/Texture.h"
#include <string>

namespace Viper
{
	namespace Graphics
	{
		class TextureLoader
		{
		public:
			TextureLoader() = default;
			virtual ~TextureLoader() = default;

			virtual Texture LoadTexture(const std::string& textureFile) = 0;
		};
	}
}
