#pragma once

#include <string>
#include "Core/Service.h"
#include "Graphics/Texture.h"

namespace Viper
{
	namespace Graphics
	{
		class TextureLoader : public Service
		{
		public:
			TextureLoader();
			virtual ~TextureLoader() = default;

			virtual Texture LoadTexture(const std::string& textureFile) = 0;

			static const ServiceType Type;
		};
	}
}
