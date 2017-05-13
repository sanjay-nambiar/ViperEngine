#include "Core/ModuleExports.h"
#include "Core/ServiceLocator.h"
#include "GlfwInputManager.h"
#include "GlfwWindowManager.h"

using namespace Viper;

void InitializeModule(Viper::ServiceLocator& serviceLocator, const std::unordered_map<std::string, std::string>&)
{
	MemoryAllocator& allocator = serviceLocator.GetMemoryAllocator();

	void* memBlock = allocator.Allocate(sizeof(Window::GlfwWindowManager), 1);
	assert(memBlock != nullptr);
	WindowManager* windowManager = new(memBlock) Window::GlfwWindowManager();
	serviceLocator.Provide(*windowManager);

	memBlock = allocator.Allocate(sizeof(Input::GlfwInputManager), 1);
	assert(memBlock != nullptr);
	InputManager* inputManager = new(memBlock) Input::GlfwInputManager();
	serviceLocator.Provide(*inputManager);
}
