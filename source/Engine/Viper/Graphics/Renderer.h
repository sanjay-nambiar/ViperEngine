#pragma once

#include <unordered_map>
#include <vector>
#include "glm/glm.hpp"
#include "Core/Service.h"
#include "Core/Types.h"
#include "Graphics/RendererTypes.h"
#include "Graphics/Shader.h"
#include "Window/WindowContext.h"

namespace Viper
{
	namespace Gameplay
	{
		class Actor;
	}

	namespace Graphics
	{
		class Mesh;

		class Renderer : public Service
		{
		public:
			Renderer();
			virtual ~Renderer() = default;

			virtual void Initialize() = 0;
			virtual void SetViewport(const Window::WindowContext& windowContext) = 0;

			virtual Graphics::Shader LoadShaderSource(const std::string& shaderSource, Graphics::ShaderType shaderType) = 0;
			virtual Graphics::Shader LoadShaderFile(const std::string& shaderFile, Graphics::ShaderType shaderType) = 0;
			virtual void DeleteShader(const Graphics::Shader& shader) = 0;
			virtual void UseShader(const Graphics::Shader& shader) = 0;
			virtual void UseShaders(const std::vector<Graphics::Shader>& shaders) = 0;

			virtual Graphics::GpuTextureResource* CreateTextureResource(const Graphics::TextureDescription& description) = 0;
			virtual bool FreeTextureResource(Graphics::GpuTextureResource& resource) = 0;

			virtual void LoadMesh(const Graphics::Mesh& mesh) = 0;
			virtual void AddActorToScene(const Gameplay::Actor& actor) = 0;

			virtual void Update();
			virtual void Shutdown() = 0;

			void AddPoint(const glm::vec3& point, Graphics::Primitive primitive);
			void AddPoints(const std::vector<glm::vec3> points, Graphics::Primitive primitive);

			static const ServiceType Type;
		private:
			std::unordered_map<Graphics::Primitive, std::vector<glm::vec3>> points;
			std::vector<Graphics::GpuTextureResource*> resources;
		};
	}
}
