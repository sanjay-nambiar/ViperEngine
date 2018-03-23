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

	auto memBlock1 = allocator.Allocate<Window::GlfwWindowManager>(1);
	assert(memBlock1 != nullptr);
	WindowManager* windowManager = new(memBlock1) Window::GlfwWindowManager();
	serviceLocator.Provide(*windowManager);

	auto memBlock2 = allocator.Allocate<Input::GlfwInputManager>(1);
	assert(memBlock2 != nullptr);
	InputManager* inputManager = new(memBlock2) Input::GlfwInputManager();
	serviceLocator.Provide(*inputManager);
}
