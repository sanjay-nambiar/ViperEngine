#include "Pch.h"
#include "Texture.h"

namespace Viper
{
	namespace Graphics
	{
		Texture::Texture(std::uint32_t width, std::uint32_t height, std::uint32_t textureId) :
			mWidth(width), mHeight(height), mTextureId(textureId)
		{
		}

		bool Texture::operator==(const Texture& rhs) const
		{
			return (mTextureId == rhs.mTextureId && mWidth == rhs.mWidth && mHeight == rhs.mHeight);
		}

		bool Texture::operator!=(const Texture& rhs) const
		{
			return !(operator==(rhs));
		}

		std::uint32_t Texture::TextureId() const
		{
			return mTextureId;
		}
	}
}
