#pragma once

#include "Core/ServiceLocator.h"
#include "Graphics/Renderer.h"

namespace Viper
{
	namespace Graphics
	{
		class VulkanRendererPrivate;

		class VulkanRenderer : public Renderer
		{
		public:
			VulkanRenderer(ServiceLocator& serviceLocator);
			~VulkanRenderer() = default;

			void Initialize() override;
			void SetViewport(const Window::WindowContext& windowContext) override;

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
			VulkanRendererPrivate* vulkanRendererPrivate;
		};
	}
}
