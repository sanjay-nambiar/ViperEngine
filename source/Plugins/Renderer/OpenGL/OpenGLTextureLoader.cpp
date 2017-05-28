#include "OpenGLTextureLoader.h"
#include "SOIL.h"
#include "glad/glad.h"

namespace Viper
{
	namespace Renderer
	{
		Graphics::Texture OpenGLTextureLoader::LoadTexture(const std::string& textureFile)
		{
			//TODO: The loader has a side effect that it also binds the loaded texture. Get rid of this
			GLuint textureId;
			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);

			GLint width, height;
			unsigned char* image = SOIL_load_image(textureFile.c_str(), &width, &height, nullptr, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);

			return Graphics::Texture(width, height, textureId);
		}
	}
}
