#include "VulkanRenderer.h"

using namespace std;
using namespace Viper::Window;
using namespace Viper::Gameplay;

namespace Viper
{
	namespace Graphics
	{
		VulkanRenderer::VulkanRenderer(ServiceLocator& serviceLocator) :
			serviceLocator(serviceLocator)
		{
		}

		void VulkanRenderer::Initialize()
		{
		}

		void VulkanRenderer::SetViewport(const WindowContext&)
		{
		}

		Shader VulkanRenderer::LoadShaderSource(const string&, ShaderType)
		{
			return Shader();
		}

		Shader VulkanRenderer::LoadShaderFile(const string&, ShaderType)
		{
			return Shader();
		}

		void VulkanRenderer::DeleteShader(const Shader&)
		{
		}

		void VulkanRenderer::UseShader(const Shader&)
		{
		}

		void VulkanRenderer::UseShaders(const vector<Shader>&)
		{
		}

		GpuTextureResource* VulkanRenderer::CreateTextureResource(const TextureDescription&)
		{
		}

		bool VulkanRenderer::FreeTextureResource(GpuTextureResource&)
		{
		}

		void VulkanRenderer::LoadMesh(const Mesh&)
		{
		}

		void VulkanRenderer::AddActorToScene(const Actor&)
		{
		}

		void VulkanRenderer::Update()
		{
		}

		void VulkanRenderer::Shutdown()
		{
		}
	}
}
