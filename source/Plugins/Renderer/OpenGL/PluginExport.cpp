#include "Core/ModuleExports.h"
#include "Core/ServiceLocator.h"
#include "OpenGLTextureLoader.h"
#include "OpenGLRenderer.h"

using namespace std;
using namespace Viper;
using namespace Viper::Graphics;
using namespace Viper::Memory;

void InitializeModule(ServiceLocator& serviceLocator, const unordered_map<string, string>&)
{
	MemoryAllocator& allocator = serviceLocator.GetMemoryAllocator();

	void* memBlock = allocator.Allocate(sizeof(OpenGLRenderer), 1);
	assert(memBlock != nullptr);
	OpenGLRenderer* rendererSystem = new(memBlock) OpenGLRenderer(serviceLocator);
	serviceLocator.Provide(*rendererSystem);

	void* memBlock2 = allocator.Allocate(sizeof(OpenGLTextureLoader), 1);
	assert(memBlock2 != nullptr);
	OpenGLTextureLoader* textureLoader = new(memBlock2) OpenGLTextureLoader();
	serviceLocator.Provide(*textureLoader);
}
