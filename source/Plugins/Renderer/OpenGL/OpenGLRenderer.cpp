#include "OpenGLRenderer.h"
#include <stdexcept>
#include "ShaderCompiler.h"
#include "Graphics/Texture.h"
#include "Core/ServiceLocator.h"
#include "OpenGLTextureLoader.h"

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
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			// Create shader program and use it
			GLuint vertexShader = ShaderCompiler::CompileShaderFromFile("Content/Shaders/default.vert", ShaderCompiler::ShaderType::VERTEX_SHADER);
			GLuint fragmentShader = ShaderCompiler::CompileShaderFromFile("Content/Shaders/default.frag", ShaderCompiler::ShaderType::FRAGMENT_SHADER);
			std::vector<GLuint> shaders = {vertexShader, fragmentShader};
			shaderProgram = ShaderCompiler::CreateProgramWithShaders(shaders);
			glBindFragDataLocation(shaderProgram, 0, "outColor");
			glUseProgram(shaderProgram);

			// free shaders since program is created
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// vertices and indices to vertices for a tringle
			GLfloat vertices[] = {
				-0.5f,  0.5f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Top-left
				0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Top-right
				0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Bottom-right
				-0.5f, -0.5f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // Bottom-left
			};

			GLuint elements[] = {
				0, 1, 2,
				2, 3, 0
			};

			// Create vertex attrib object and bind it
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			// create VertexBufferObject and bind it and copy data
			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// create VertexBufferObject and bind it and copy data
			GLuint EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


			// configure the size and stride attributes of vertex buffer object and element buffer object
			GLint positionAttribute = glGetAttribLocation(shaderProgram, "position");
			glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), nullptr);
			glEnableVertexAttribArray(positionAttribute);

			GLint colorAttribute = glGetAttribLocation(shaderProgram, "color");
			glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), reinterpret_cast<void*>(2 * sizeof(GLfloat)));
			glEnableVertexAttribArray(colorAttribute);


			GLint textureAttribute = glGetAttribLocation(shaderProgram, "texCoord");
			glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), reinterpret_cast<void*>(5 * sizeof(GLfloat)));
			glEnableVertexAttribArray(textureAttribute);


			// Create and bind texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			OpenGLTextureLoader loader;
			Graphics::Texture textureObj = loader.LoadTexture("Content/Textures/wall.jpg");

			// Set texture wrapping options in each coordinate (x, y, z) = (s, t, r)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			// Border color for texture if wrapping is clamp_to_border
			float color[] = {1.0f, 0.0f, 0.0f, 1.0f};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

			// Generate mipmaps
			glGenerateMipmap(GL_TEXTURE_2D);
			// Texture scaling option (for scaling up and down)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		}

		void OpenGLRenderer::SetViewport(const WindowContext& windowContext)
		{
			glViewport(windowContext.x, windowContext.y, windowContext.width, windowContext.height);
		}

		void OpenGLRenderer::Update()
		{
			RendererSystem::Update();
			glClear(GL_COLOR_BUFFER_BIT);

			auto now = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start).count();

			GLuint modifier = glGetUniformLocation(shaderProgram, "modifier");
			glUniform1f(modifier, ((sin(time * 4.0f) + 1.0f) / 2.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		void OpenGLRenderer::Shutdown()
		{
		}
	}
}