#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "Core/Vector3.h"
#include "Window/WindowContext.h"
#include "Graphics/Shader.h"

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

	struct TextureDescription
	{
		std::uint32_t target;
		std::int32_t level;
		std::int32_t internalFormat;
		std::uint32_t width;
		std::uint32_t height;
		std::uint32_t format;
		std::uint32_t type;
		void* data;

		std::uint32_t wrapS;
		std::uint32_t wrapT;
		std::uint32_t minFilter;
		std::uint32_t magFilter;
		TextureDescription();
	};

	struct GpuTextureResource
	{
	};

	class RendererSystem
	{
	public:
		enum class Primitive : std::uint32_t
		{
			Point,
			Line,
			Triangle
		};

		RendererSystem();
		virtual ~RendererSystem() = default;

		virtual void Initialize() = 0;
		virtual void SetViewport(const WindowContext& windowContext) = 0;

		virtual Graphics::Shader LoadShaderSource(const std::string& shaderSource, Graphics::ShaderType shaderType) = 0;
		virtual Graphics::Shader LoadShaderFile(const std::string& shaderFile, Graphics::ShaderType shaderType) = 0;
		virtual void DeleteShader(const Graphics::Shader& shader) = 0;
		virtual void UseShader(const Graphics::Shader& shader) = 0;
		virtual void UseShaders(const std::vector<Graphics::Shader>& shaders) = 0;

		virtual GpuTextureResource* CreateTextureResource(const TextureDescription& description) = 0;
		virtual bool FreeTextureResource(GpuTextureResource& resource) = 0;

		virtual void LoadMesh(const Graphics::Mesh& mesh) = 0;
		virtual void AddActorToScene(const Gameplay::Actor& actor) = 0;

		virtual void Update();
		virtual void Shutdown() = 0;

		void AddPoint(const Vector3& point, Primitive primitive);
		void AddPoints(const std::vector<Vector3> points, Primitive primitive);

	private:
		std::unordered_map<Primitive, std::vector<Vector3>> points;
		std::vector<GpuTextureResource*> resources;
	};
}
