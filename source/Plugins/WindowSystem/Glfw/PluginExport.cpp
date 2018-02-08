#include "Core/ServiceLocator.h"
#include "Platform/ModuleExports.h"
#include "GlfwInputManager.h"
#include "GlfwWindowManager.h"

using namespace std;
using namespace Viper;
using namespace Viper::Memory;
using namespace Viper::Window;
using namespace Viper::Input;

void InitializeModule(Viper::ServiceLocator& serviceLocator, const std::unordered_map<std::string, std::string>&)
{
	auto& allocator = serviceLocator.Get<MemoryAllocator>();

	void* memBlock = allocator.Allocate(sizeof(Window::GlfwWindowManager), 1);
	assert(memBlock != nullptr);
	WindowManager* windowManager = new(memBlock) Window::GlfwWindowManager();
	serviceLocator.Provide(*windowManager);

	memBlock = allocator.Allocate(sizeof(Input::GlfwInputManager), 1);
	assert(memBlock != nullptr);
	InputManager* inputManager = new(memBlock) Input::GlfwInputManager();
	serviceLocator.Provide(*inputManager);
}
