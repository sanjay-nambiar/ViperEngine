#include "OpenGLRenderer.h"
#include <stdexcept>
#include "ShaderCompiler.h"
#include <chrono>

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
			GLuint vertexShader = ShaderCompiler::CompileShaderFromFile("Content\\Shaders\\default.vert", ShaderCompiler::ShaderType::VERTEX_SHADER);
			GLuint fragmentShader = ShaderCompiler::CompileShaderFromFile("Content\\Shaders\\default.frag", ShaderCompiler::ShaderType::FRAGMENT_SHADER);
			std::vector<GLuint> shaders = {vertexShader, fragmentShader};
			shaderProgram = ShaderCompiler::CreateProgramWithShaders(shaders);
			glBindFragDataLocation(shaderProgram, 0, "outColor");
			glUseProgram(shaderProgram);

			// free shaders since program is created
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// vertices and indices to vertices for a tringle
			GLfloat vertices[] = {
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				0.0f,  0.5f, 0.0f
			};

			// create VertexBufferObject and bind it and copy data
			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// Create vertex attrib object and bind it
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			// configure the size and stride attributes of vertex buffer object and element buffer object
			GLint positionAttribute = glGetAttribLocation(shaderProgram, "position");
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(positionAttribute);

			start = std::chrono::high_resolution_clock::now();
			alpha = glGetUniformLocation(shaderProgram, "modifier");
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

			glUniform1f(alpha, (sin(time * 4.0f) + 1.0f));
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		void OpenGLRenderer::Shutdown()
		{
		}
	}
}
