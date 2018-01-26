#include "OpenGLRenderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Gameplay/Actor.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "ShaderCompiler.h"

namespace Viper
{
	namespace Renderer
	{
		using namespace Graphics;

		OpenGLRenderer::OpenGLRenderer() :
			activeShaderProgram(0)
		{
		}

		void OpenGLRenderer::Initialize()
		{
			if (!gladLoadGL())
			{
				throw std::runtime_error("Unable to initialize OpenGL");
			}
			glEnable(GL_DEPTH_TEST);
		}

		void OpenGLRenderer::SetViewport(const WindowContext& windowContext)
		{
			glViewport(windowContext.x, windowContext.y, windowContext.width, windowContext.height);
		}

		Graphics::Shader OpenGLRenderer::LoadShaderSource(const std::string& shaderSource, Graphics::ShaderType shaderType)
		{
			return ShaderCompiler::CompileShader(shaderSource, shaderType);
		}

		Graphics::Shader OpenGLRenderer::LoadShaderFile(const std::string& shaderFile, Graphics::ShaderType shaderType)
		{
			return ShaderCompiler::CompileShaderFromFile(shaderFile, shaderType);
		}

		void OpenGLRenderer::DeleteShader(const Graphics::Shader& shader)
		{
			glDetachShader(activeShaderProgram, shader.Id());
			glDeleteShader(shader.Id());
		}

		void OpenGLRenderer::UseShader(const Graphics::Shader& shader)
		{
			glDeleteProgram(activeShaderProgram);
			activeShaders[shader.Type()] = shader;
			GLuint program = ShaderCompiler::CreateProgramWithShaders(activeShaders);
			glUseProgram(program);
			activeShaderProgram = program;
		}

		void OpenGLRenderer::UseShaders(const std::vector<Graphics::Shader>& shaders)
		{
			glDeleteProgram(activeShaderProgram);
			for (const auto& shader : shaders)
			{
				activeShaders[shader.Type()] = shader;
			}
			GLuint program = ShaderCompiler::CreateProgramWithShaders(activeShaders);
			glUseProgram(program);
			activeShaderProgram = program;
		}

		GpuTextureResource* OpenGLRenderer::CreateTextureResource(const TextureDescription& description)
		{
			OpenGLTextureResource* resource = new OpenGLTextureResource();
			glGenTextures(1, &resource->id);
			glTexImage2D(description.target, description.level, description.internalFormat, description.width, description.height, 0, description.format, description.type, description.data);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, resource->id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			return resource;
		}

		bool OpenGLRenderer::FreeTextureResource(GpuTextureResource& resource)
		{
			glDeleteTextures(1, &reinterpret_cast<OpenGLTextureResource&>(resource).id);
			return true;
		}

		void OpenGLRenderer::LoadMesh(const Graphics::Mesh& mesh)
		{
			// Create vertex attrib object and bind it
			GLuint VAO;
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			// create VertexBufferObject and bind it and copy data
			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, mesh.VertexDataSize(), mesh.VertexData(), GL_STATIC_DRAW);

			// create VertexBufferObject and bind it and copy data
			GLuint EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.TriDataSize(), mesh.TriData(), GL_STATIC_DRAW);

			// configure the size and stride attributes of vertex buffer object and element buffer object
			GLint positionAttribute = glGetAttribLocation(activeShaderProgram, "position");
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
			glEnableVertexAttribArray(positionAttribute);	

			GLint colorAttribute = glGetAttribLocation(activeShaderProgram, "color");
			glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(colorAttribute);

			GLint textureAttribute = glGetAttribLocation(activeShaderProgram, "texCoord");
			glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(textureAttribute);

			meshes[&mesh] = VAO;
		}

		void OpenGLRenderer::AddActorToScene(const Gameplay::Actor& actor)
		{
			actors.push_back(&actor);
		}

		void OpenGLRenderer::Update()
		{
			RendererSystem::Update();
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto now = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start).count();

			GLuint modifier = glGetUniformLocation(activeShaderProgram, "modifier");
			glUniform1f(modifier, ((sin(time * 4.0f) + 1.0f) / 2.0f));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 1);
			glUniform1i(glGetUniformLocation(activeShaderProgram, "textureSample1"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 2);
			glUniform1i(glGetUniformLocation(activeShaderProgram, "textureSample2"), 1);

			GLuint model= glGetUniformLocation(activeShaderProgram, "model");
			GLuint view = glGetUniformLocation(activeShaderProgram, "view");
			GLuint projection = glGetUniformLocation(activeShaderProgram, "projection");

			for (const auto& actor : actors)
			{
				glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(actor->Model()));
				glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(actor->View()));
				glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(actor->Projection()));

				glBindVertexArray(meshes[&actor->Mesh()]);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
			}
		}

		void OpenGLRenderer::Shutdown()
		{
		}
	}
}
