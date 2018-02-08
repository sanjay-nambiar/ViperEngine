#pragma once

#include <istream>
#include "Core/Types.h"

namespace Viper
{
	namespace Graphics
	{
		class Texture final
		{
		public:
			Texture(std::uint32_t width, std::uint32_t height, std::uint32_t textureId);
			~Texture() = default;

			bool operator==(const Texture& rhs) const;
			bool operator!=(const Texture& rhs) const;

			std::uint32_t TextureId() const;
		private:
			std::uint32_t mWidth;
			std::uint32_t mHeight;
			std::uint32_t mTextureId;
		};
	}
}
