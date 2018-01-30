#include "OpenGLTextureLoader.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

namespace Viper
{
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
			float color[] = {1.0f, 0.0f, 0.0f, 1.0f};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

			GLint width, height, channels;
			unsigned char* image = stbi_load(textureFile.c_str(), &width, &height, &channels, 3);
			if (image == nullptr)
			{
				throw runtime_error(string("Unable to load texture file ") + textureFile);
			}
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			stbi_image_free(image);

			// Generate mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);
			// Texture scaling option (for scaling up and down)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			glBindTexture(GL_TEXTURE_2D, 0);
			return Texture(width, height, textureId);
		}
	}
}
