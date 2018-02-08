#pragma once

#include "Graphics/Renderer.h"
#include <chrono>
#include <unordered_map>
#include "glad/glad.h"
#include "Window/WindowContext.h"
#include "Core/ServiceLocator.h"

struct GLFWwindow;

namespace Viper
{
	namespace Graphics
	{
		struct OpenGLTextureResource : public GpuTextureResource
		{
			uint32_t id;
		};

		class OpenGLRenderer final : public Renderer
		{
		public:
			OpenGLRenderer(ServiceLocator& serviceLocator);
			~OpenGLRenderer() = default;

			void Initialize() override;
			void SetViewport(const WindowContext& windowContext) override;

			Shader LoadShaderSource(const std::string& shaderSource, ShaderType shaderType) override;
			Shader LoadShaderFile(const std::string& shaderFile, ShaderType shaderType) override;
			void DeleteShader(const Shader& shader) override;
			void UseShader(const Shader& shader) override;
			void UseShaders(const std::vector<Shader>& shaders) override;

			GpuTextureResource* CreateTextureResource(const TextureDescription& description) override;
			bool FreeTextureResource(GpuTextureResource& resource) override;

			void LoadMesh(const Mesh& mesh) override;
			void AddActorToScene(const Gameplay::Actor& actor) override;

			void Update() override;
			void Shutdown() override;

		private:
			ServiceLocator& serviceLocator;
			std::unordered_map<ShaderType, Shader> activeShaders;
			GLuint activeShaderProgram;
			std::chrono::time_point<std::chrono::steady_clock> start;

			std::unordered_map<const Mesh*, GLuint> meshes;
			std::vector<const Gameplay::Actor*> actors;
		};
	}
}
