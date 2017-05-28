#include "Core/ModuleExports.h"
#include "Core/ServiceLocator.h"
#include "OpenGLTextureLoader.h"
#include "OpenGLRenderer.h"

void InitializeModule(Viper::ServiceLocator& serviceLocator, const std::unordered_map<std::string, std::string>&)
{
	Viper::MemoryAllocator& allocator = serviceLocator.GetMemoryAllocator();

	void* memBlock = allocator.Allocate(sizeof(Viper::Renderer::OpenGLRenderer), 1);
	assert(memBlock != nullptr);
	Viper::Renderer::OpenGLRenderer* rendererSystem = new(memBlock) Viper::Renderer::OpenGLRenderer();
	serviceLocator.Provide(*rendererSystem);

	void* memBlock2 = allocator.Allocate(sizeof(Viper::Renderer::OpenGLTextureLoader), 1);
	assert(memBlock2 != nullptr);
	Viper::Renderer::OpenGLTextureLoader* textureLoader = new(memBlock2) Viper::Renderer::OpenGLTextureLoader();
	serviceLocator.Provide(*textureLoader);
}
