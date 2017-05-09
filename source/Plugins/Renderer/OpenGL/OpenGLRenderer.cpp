#include "OpenGLRenderer.h"
#include "glad/glad.h"
#include <stdexcept>

namespace Viper
{
	namespace Renderer
	{
		void OpenGLRenderer::Initialize()
		{
			if (!gladLoadGL())
			{
				throw std::runtime_error("Unable to initialize OpenGL");
			}
		}

		void OpenGLRenderer::Update()
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void OpenGLRenderer::Shutdown()
		{
		}
	}
}
