#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "Core/Vector3.h"
#include "Window/WindowContext.h"
#include "Graphics/Shader.h"
#include "Graphics/RendererTypes.h"

namespace Viper
{
	namespace Graphics
	{
		class Mesh;
	}

	namespace Gameplay
	{
		class Actor;
	}

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer() = default;

		virtual void Initialize() = 0;
		virtual void SetViewport(const WindowContext& windowContext) = 0;

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

		void AddPoint(const Vector3& point, Graphics::Primitive primitive);
		void AddPoints(const std::vector<Vector3> points, Graphics::Primitive primitive);

	private:
		std::unordered_map<Graphics::Primitive, std::vector<Vector3>> points;
		std::vector<Graphics::GpuTextureResource*> resources;
	};
}
