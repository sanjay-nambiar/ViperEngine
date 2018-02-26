#include "glad/glad.h"
#include "OpenGLTextureLoader.h"
#include "Asset/TextureAsset.h"
#include "Core/GameException.h"

using namespace std;

namespace Viper
{
	using namespace Assets;

	namespace Graphics
	{
		Texture OpenGLTextureLoader::LoadTexture(const string& textureFile)
		{
			GLuint textureId;
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);

			// Set texture wrapping options in each coordinate (x, y, z) = (s, t, r)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			// Border color for texture if wrapping is clamp_to_border
			float32_t color[] = {1.0f, 0.0f, 0.0f, 1.0f};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

			TextureAsset asset(StringID(textureFile.c_str()));
			asset.Load();
			auto& textureData = asset.Data();
			if (textureData.image.data == nullptr)
			{
				throw GameException(string("Unable to load texture file ") + textureFile);
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData.width, textureData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.image.data);

			// Generate mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);
			// Texture scaling option (for scaling up and down)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			glBindTexture(GL_TEXTURE_2D, 0);
			return Texture(textureData.width, textureData.height, textureId);
		}
	}
}
