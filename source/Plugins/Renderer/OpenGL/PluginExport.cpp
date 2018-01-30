#include "Core/ModuleExports.h"
#include "Core/ServiceLocator.h"
#include "OpenGLTextureLoader.h"
#include "OpenGLRenderer.h"

using namespace Viper;

void InitializeModule(ServiceLocator& serviceLocator, const std::unordered_map<std::string, std::string>&)
{
	MemoryAllocator& allocator = serviceLocator.GetMemoryAllocator();

	void* memBlock = allocator.Allocate(sizeof(Renderer::OpenGLRenderer), 1);
	assert(memBlock != nullptr);
	Renderer::OpenGLRenderer* rendererSystem = new(memBlock) Renderer::OpenGLRenderer(serviceLocator);
	serviceLocator.Provide(*rendererSystem);

	void* memBlock2 = allocator.Allocate(sizeof(Renderer::OpenGLTextureLoader), 1);
	assert(memBlock2 != nullptr);
	Renderer::OpenGLTextureLoader* textureLoader = new(memBlock2) Renderer::OpenGLTextureLoader();
	serviceLocator.Provide(*textureLoader);
}
