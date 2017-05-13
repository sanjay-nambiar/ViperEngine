#include "OpenGLRenderer.h"
#include <stdexcept>
#include "ShaderCompiler.h"

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

			GLuint vertexShader = ShaderCompiler::CompileShaderFromFile("Content\\Shaders\\default.vert", ShaderCompiler::ShaderType::VERTEX_SHADER);
			GLuint fragmentShader = ShaderCompiler::CompileShaderFromFile("Content\\Shaders\\default.frag", ShaderCompiler::ShaderType::FRAGMENT_SHADER);
			std::vector<GLuint> shaders = {vertexShader, fragmentShader};
			shaderProgram = ShaderCompiler::CreateProgramWithShaders(shaders);
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
			GLuint indices[] = {
				0, 1, 2
			};

			// create VertexBufferObject, ElementBufferObject and VertexArrayObject
			GLuint VBO;
			glGenBuffers(1, &VBO);
			GLuint EBO;
			glGenBuffers(1, &EBO);
			glGenVertexArrays(1, &VAO);

			// configure the size and stride attributes of vertex buffer object and element buffer object
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
			// bind the VAO for triangle and set to use our shader program
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);
			glUseProgram(shaderProgram);
		}

		void OpenGLRenderer::SetViewport(const WindowContext& windowContext)
		{
			glViewport(windowContext.x, windowContext.y, windowContext.width, windowContext.height);
		}

		void OpenGLRenderer::Update()
		{
			RendererSystem::Update();
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}

		void OpenGLRenderer::Shutdown()
		{
		}
	}
}
