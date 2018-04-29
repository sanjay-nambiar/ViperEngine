#include "Core/ServiceLocator.h"
#include "Platform/ModuleExports.h"
#include "VulkanRenderer.h"

using namespace std;
using namespace Viper;
using namespace Viper::Graphics;
using namespace Viper::Memory;

void InitializeModule(ServiceLocator& serviceLocator, const unordered_map<string, string>&)
{
	auto& allocator = serviceLocator.Get<MemoryAllocator>();

	auto memBlock = allocator.Allocate<VulkanRenderer>(1);
	assert(memBlock != nullptr);
	VulkanRenderer* rendererSystem = new(memBlock) VulkanRenderer(serviceLocator);
	serviceLocator.Provide(*rendererSystem);
}
