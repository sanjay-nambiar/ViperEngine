#include "OpenGLRenderer.h"
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Gameplay/Actor.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include "ShaderCompiler.h"
#include "Graphics/FrameGraph.h"

using namespace std;

namespace Viper
{
	namespace Graphics
	{

		static const unordered_map<TextureType, uint32_t> TextureTypeMap = {
			{ TextureType::TEXTURE_1D,          GL_TEXTURE_1D },
			{ TextureType::TEXTURE_2D,          GL_TEXTURE_2D },
			{ TextureType::TEXTURE_3D,          GL_TEXTURE_3D },
			{ TextureType::TEXTURE_CUBE_MAP,    GL_TEXTURE_CUBE_MAP }
		};

		struct OpenGLFormatDescription
		{
			GLuint internalFormat;
			GLuint format;
			GLuint type;
		};

		static const unordered_map<TextureFormat, OpenGLFormatDescription> TextureFormatMap = {
			{ TextureFormat::D16,        { GL_DEPTH_COMPONENT16,     GL_DEPTH_COMPONENT,    GL_UNSIGNED_SHORT } },
			{ TextureFormat::D24,        { GL_DEPTH_COMPONENT24,     GL_DEPTH_COMPONENT,    GL_UNSIGNED_INT } },
			{ TextureFormat::D32F,       { GL_DEPTH_COMPONENT32F,    GL_DEPTH_COMPONENT,    GL_FLOAT } },
			{ TextureFormat::D24S8,      { GL_DEPTH24_STENCIL8,      GL_DEPTH_STENCIL,      GL_UNSIGNED_INT_24_8 } },
			{ TextureFormat::D32FS8,     { GL_DEPTH32F_STENCIL8,     GL_DEPTH_STENCIL,      GL_FLOAT_32_UNSIGNED_INT_24_8_REV } },
			{ TextureFormat::R16F,       { GL_R16F,                  GL_R,                  GL_HALF_FLOAT } },
			{ TextureFormat::RG16F,      { GL_RG16F,                 GL_RG,                 GL_HALF_FLOAT } },
			{ TextureFormat::RGB16F,     { GL_RGB16F,                GL_RGB,                GL_HALF_FLOAT } },
			{ TextureFormat::RGBA16F,    { GL_RGBA16F,               GL_RGBA,               GL_HALF_FLOAT } },
			{ TextureFormat::R32F,       { GL_R32F,                  GL_R,                  GL_FLOAT } },
			{ TextureFormat::RG32F,      { GL_RG32F,                 GL_RG,                 GL_FLOAT } },
			{ TextureFormat::RGB32F,     { GL_RGB32F,                GL_RGB,                GL_FLOAT } },
			{ TextureFormat::RGBA32F,    { GL_RGBA32F,               GL_RGBA,               GL_FLOAT } },
			{ TextureFormat::R8I,        { GL_R8I,                   GL_RED_INTEGER,        GL_BYTE } },
			{ TextureFormat::R16I,       { GL_R16I,                  GL_RED_INTEGER,        GL_SHORT } },
			{ TextureFormat::R32I,       { GL_R32I,                  GL_RED_INTEGER,        GL_INT } },
			{ TextureFormat::RG8I,       { GL_RG8I,                  GL_RG_INTEGER,         GL_BYTE } },
			{ TextureFormat::RG16I,      { GL_RG16I,                 GL_RG_INTEGER,         GL_SHORT } },
			{ TextureFormat::RG32I,      { GL_RG32I,                 GL_RG_INTEGER,         GL_INT } },
			{ TextureFormat::RGB8I,      { GL_RGB8I,                 GL_RGB_INTEGER,        GL_BYTE } },
			{ TextureFormat::RGB16I,     { GL_RGB16I,                GL_RGB_INTEGER,        GL_SHORT } },
			{ TextureFormat::RGB32I,     { GL_RGB32I,                GL_RGB_INTEGER,        GL_INT } },
			{ TextureFormat::RGBA8I,     { GL_RGBA8I,                GL_RGBA_INTEGER,       GL_BYTE } },
			{ TextureFormat::RGBA16I,    { GL_RGBA16I,               GL_RGBA_INTEGER,       GL_SHORT } },
			{ TextureFormat::RGBA32I,    { GL_RGBA32I,               GL_RGBA_INTEGER,       GL_INT } },
			{ TextureFormat::R8UI,       { GL_R8UI,                  GL_RED_INTEGER,        GL_UNSIGNED_BYTE } },
			{ TextureFormat::R16UI,      { GL_R16UI,                 GL_RED_INTEGER,        GL_UNSIGNED_SHORT } },
			{ TextureFormat::R32UI,      { GL_R32UI,                 GL_RED_INTEGER,        GL_UNSIGNED_INT } },
			{ TextureFormat::RG8UI,      { GL_RG8UI,                 GL_RG_INTEGER,         GL_UNSIGNED_BYTE } },
			{ TextureFormat::RG16UI,     { GL_RG16UI,                GL_RG_INTEGER,         GL_UNSIGNED_SHORT } },
			{ TextureFormat::RG32UI,     { GL_RG32UI,                GL_RG_INTEGER,         GL_UNSIGNED_INT } },
			{ TextureFormat::RGB8UI,     { GL_RGB8UI,                GL_RGB_INTEGER,        GL_UNSIGNED_BYTE } },
			{ TextureFormat::RGB16UI,    { GL_RGB16UI,               GL_RGB_INTEGER,        GL_UNSIGNED_SHORT } },
			{ TextureFormat::RGB32UI,    { GL_RGB32UI,               GL_RGB_INTEGER,        GL_UNSIGNED_INT } },
			{ TextureFormat::RGBA8UI,    { GL_RGBA8UI,               GL_RGBA_INTEGER,       GL_UNSIGNED_BYTE } },
			{ TextureFormat::RGBA16UI,   { GL_RGBA16UI,              GL_RGBA_INTEGER,       GL_UNSIGNED_SHORT } },
			{ TextureFormat::RGBA32UI,   { GL_RGBA32UI,              GL_RGBA_INTEGER,       GL_UNSIGNED_INT } }
		};

		static const unordered_map<TextureWrapping, uint32_t> TextureWrappingMap = {
			{ TextureWrapping::CLAMP_TO_EDGE,           GL_CLAMP_TO_EDGE },
			{ TextureWrapping::CLAMP_TO_BORDER,         GL_CLAMP_TO_BORDER },
			{ TextureWrapping::REPEAT,                  GL_REPEAT },
			{ TextureWrapping::MIRRORED_REPEAT,         GL_MIRRORED_REPEAT },
			{ TextureWrapping::MIRROR_CLAMP_TO_EDGE,    GL_MIRROR_CLAMP_TO_EDGE }
		};

		static const unordered_map<TextureFiltering, uint32_t> TextureFilteringMap = {
			{ TextureFiltering::NEAREST,                   GL_NEAREST },
			{ TextureFiltering::LINEAR,                    GL_LINEAR },
			{ TextureFiltering::NEAREST_MIPMAP_NEAREST,    GL_NEAREST_MIPMAP_NEAREST },
			{ TextureFiltering::LINEAR_MIPMAP_NEAREST,     GL_LINEAR_MIPMAP_NEAREST },
			{ TextureFiltering::NEAREST_MIPMAP_LINEAR,     GL_NEAREST_MIPMAP_LINEAR },
			{ TextureFiltering::LINEAR_MIPMAP_LINEAR,      GL_LINEAR_MIPMAP_LINEAR }
		};

		OpenGLRenderer::OpenGLRenderer(ServiceLocator& serviceLocator) :
			serviceLocator(serviceLocator), activeShaderProgram(0)
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

		Shader OpenGLRenderer::LoadShaderSource(const string& shaderSource, ShaderType shaderType)
		{
			return ShaderCompiler::CompileShader(shaderSource, shaderType);
		}

		Shader OpenGLRenderer::LoadShaderFile(const string& shaderFile, ShaderType shaderType)
		{
			return ShaderCompiler::CompileShaderFromFile(shaderFile, shaderType);
		}

		void OpenGLRenderer::DeleteShader(const Shader& shader)
		{
			glDetachShader(activeShaderProgram, shader.Id());
			glDeleteShader(shader.Id());
		}

		void OpenGLRenderer::UseShader(const Shader& shader)
		{
			glDeleteProgram(activeShaderProgram);
			activeShaders[shader.Type()] = shader;
			GLuint program = ShaderCompiler::CreateProgramWithShaders(activeShaders);
			glUseProgram(program);
			activeShaderProgram = program;
		}

		void OpenGLRenderer::UseShaders(const vector<Shader>& shaders)
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
			const auto& target = TextureTypeMap.at(description.type);
			const auto& format = TextureFormatMap.at(description.format);
			glTexImage2D(target, 0, format.internalFormat, description.width, description.height, 0, format.format,
				format.type, description.data);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(target, resource->id);
			glTexParameteri(target, GL_TEXTURE_WRAP_S, TextureWrappingMap.at(description.wrapS));
			glTexParameteri(target, GL_TEXTURE_WRAP_T, TextureWrappingMap.at(description.wrapT));
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, TextureFilteringMap.at(description.magFilter));
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, TextureFilteringMap.at(description.minFilter));
			return resource;
		}

		bool OpenGLRenderer::FreeTextureResource(GpuTextureResource& resource)
		{
			glDeleteTextures(1, &reinterpret_cast<OpenGLTextureResource&>(resource).id);
			return true;
		}

		void OpenGLRenderer::LoadMesh(const Mesh& mesh)
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
			Renderer::Update();
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

			GLuint model = glGetUniformLocation(activeShaderProgram, "model");
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
