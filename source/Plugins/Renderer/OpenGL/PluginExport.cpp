#include "Core/ModuleExports.h"
#include "Core/ServiceLocator.h"
#include "OpenGLRenderer.h"

void ProvideRenderer(Viper::ServiceLocator& serviceLocator)
{
	Viper::MemoryAllocator& allocator = serviceLocator.GetMemoryAllocator();
	void* memBlock = allocator.Allocate(sizeof(Viper::Renderer::OpenGLRenderer), 1);
	assert(memBlock != nullptr);
	Viper::Renderer::OpenGLRenderer* rendererSystem = new(memBlock) Viper::Renderer::OpenGLRenderer();
	serviceLocator.Provide(*rendererSystem);
}
