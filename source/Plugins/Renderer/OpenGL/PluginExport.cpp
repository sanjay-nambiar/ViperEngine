#include "Core/ServiceLocator.h"
#include "OpenGLTextureLoader.h"
#include "OpenGLRenderer.h"
#include "Platform/ModuleExports.h"

using namespace std;
using namespace Viper;
using namespace Viper::Graphics;
using namespace Viper::Memory;

void InitializeModule(ServiceLocator& serviceLocator, const unordered_map<string, string>&)
{
	auto& allocator = serviceLocator.Get<MemoryAllocator>();

	auto memBlock = allocator.Allocate<OpenGLRenderer>(1);
	assert(memBlock != nullptr);
	OpenGLRenderer* rendererSystem = new(memBlock) OpenGLRenderer(serviceLocator);
	serviceLocator.Provide(*rendererSystem);

	auto memBlock2 = allocator.Allocate<OpenGLTextureLoader>(1);
	assert(memBlock2 != nullptr);
	OpenGLTextureLoader* textureLoader = new(memBlock2) OpenGLTextureLoader();
	serviceLocator.Provide(*textureLoader);
}
