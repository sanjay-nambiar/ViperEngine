#include "Core/ModuleExports.h"
#include "Core/ServiceLocator.h"
#include "GlfwWindowManager.h"


using namespace Viper;


void ProvideWindowSystem(Viper::ServiceLocator& serviceLocator)
{
	MemoryAllocator& allocator = serviceLocator.GetMemoryAllocator();
	void* memBlock = allocator.Allocate(sizeof(Window::GlfwWindowManager), 1);
	assert(memBlock != nullptr);
	WindowManager* windowManager = new(memBlock) Window::GlfwWindowManager();
	serviceLocator.Provide(*windowManager);
}
