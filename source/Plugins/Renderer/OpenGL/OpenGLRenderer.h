#pragma once

#include "Service/RendererSystem.h"
#include <chrono>
#include <unordered_map>
#include "glad/glad.h"
#include "Window/WindowContext.h"
#include "Core/ServiceLocator.h"

struct GLFWwindow;

namespace Viper
{
	namespace Renderer
	{
		struct OpenGLTextureResource : public Graphics::GpuTextureResource
		{
			uint32_t id;
		};

		class OpenGLRenderer final : public RendererSystem
		{
		public:
			OpenGLRenderer(ServiceLocator& serviceLocator);
			~OpenGLRenderer() = default;

			void Initialize() override;
			void SetViewport(const WindowContext& windowContext) override;

			Graphics::Shader LoadShaderSource(const std::string& shaderSource, Graphics::ShaderType shaderType) override;
			Graphics::Shader LoadShaderFile(const std::string& shaderFile, Graphics::ShaderType shaderType) override;
			void DeleteShader(const Graphics::Shader& shader) override;
			void UseShader(const Graphics::Shader& shader) override;
			void UseShaders(const std::vector<Graphics::Shader>& shaders) override;

			Graphics::GpuTextureResource* CreateTextureResource(const Graphics::TextureDescription& description) override;
			bool FreeTextureResource(Graphics::GpuTextureResource& resource) override;

			void LoadMesh(const Graphics::Mesh& mesh) override;
			void AddActorToScene(const Gameplay::Actor& actor) override;

			void Update() override;
			void Shutdown() override;

		private:
			ServiceLocator& serviceLocator;
			std::unordered_map<Graphics::ShaderType, Graphics::Shader> activeShaders;
			GLuint activeShaderProgram;
			std::chrono::time_point<std::chrono::steady_clock> start;

			std::unordered_map<const Graphics::Mesh*, GLuint> meshes;
			std::vector<const Gameplay::Actor*> actors;
		};
	}
}
