#pragma once

#include "Service/RendererSystem.h"
#include <chrono>
#include <unordered_map>
#include "glad/glad.h"
#include "Window/WindowContext.h"



struct GLFWwindow;

namespace Viper
{
	namespace Renderer
	{
		class OpenGLRenderer final : public RendererSystem
		{
		public:
			OpenGLRenderer();
			~OpenGLRenderer() = default;

			void Initialize() override;
			void SetViewport(const WindowContext& windowContext) override;

			Graphics::Shader LoadShaderSource(const std::string& shaderSource, Graphics::ShaderType shaderType) override;
			Graphics::Shader LoadShaderFile(const std::string& shaderFile, Graphics::ShaderType shaderType) override;
			void DeleteShader(const Graphics::Shader& shader) override;
			void UseShader(const Graphics::Shader& shader) override;
			void UseShaders(const std::vector<Graphics::Shader>& shaders) override;

			void LoadMesh(const Graphics::Mesh& mesh) override;
			void Update() override;
			void Shutdown() override;

		private:
			std::unordered_map<Graphics::ShaderType, Graphics::Shader> activeShaders;
			GLuint activeShaderProgram;
			GLuint VAO;
			std::chrono::time_point<std::chrono::steady_clock> start;
		};
	}
}
