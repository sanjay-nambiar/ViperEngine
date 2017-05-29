#include "OpenGLTextureLoader.h"
#include "SOIL.h"
#include "glad/glad.h"

namespace Viper
{
	namespace Renderer
	{
		Graphics::Texture OpenGLTextureLoader::LoadTexture(const std::string& textureFile)
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

			GLint width, height;
			unsigned char* image = SOIL_load_image(textureFile.c_str(), &width, &height, nullptr, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);

			// Generate mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);
			// Texture scaling option (for scaling up and down)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			glBindTexture(GL_TEXTURE_2D, 0);
			return Graphics::Texture(width, height, textureId);
		}
	}
}
