#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "ProgramMain.h"
#include "Audio/AudioManager.h"
#include "Core/ServiceLocator.h"
#include "Gameplay/Game.h"
#include "Gameplay/GameTime.h"
#include "Graphics/Renderer.h"
#include "Input/InputManager.h"
#include "Platform/ModuleLoader.h"
#include "Window/WindowManager.h"

namespace Viper
{
	using namespace std;
	using namespace Audio;
	using namespace Gameplay;
	using namespace Input;
	using namespace Memory;
	using namespace Graphics;
	using namespace Window;

	namespace Platform
	{
		const string ProgramMain::ConfigFile = "Config.json";

		void ProgramMain::Initialize()
		{
			MemoryManager* allocator = new MemoryManager();
			assert(allocator != nullptr);
			ModuleLoader::CreateInstance(*allocator);
			ModuleLoader::GetInstance().LoadModules(ConfigFile);

			auto& serviceLocator = ServiceLocator::GetInstance();
			auto& game = serviceLocator.Get<Game>();

			auto& windowManager = serviceLocator.Get<WindowManager>();
			windowManager.Initialize();
			windowContext = &windowManager.CreateGameWindow(game.Width(), game.Height(), game.Title());

			auto& inputManager = serviceLocator.Get<InputManager>();
			inputManager.Initialize(*windowContext);

			auto& renderer = serviceLocator.Get<Renderer>();
			renderer.Initialize();
			renderer.SetViewport(*windowContext);

			game.Initialize();
		}

		void ProgramMain::Update()
		{
			auto& serviceLocator = ServiceLocator::GetInstance();
			auto& windowManager = serviceLocator.Get<WindowManager>();
			auto& game = serviceLocator.Get<Game>();
			GameTime time;
			while (windowManager.BeginUpdate(*windowContext))
			{
				if (!game.Update(time))
				{
					break;
				}
				windowManager.EndUpdate(*windowContext);
			}
		}

		void ProgramMain::Shutdown()
		{
			auto& serviceLocator = ServiceLocator::GetInstance();
			serviceLocator.Get<Renderer>().Shutdown();
			serviceLocator.Get<WindowManager>().Shutdown();

			auto& allocator = serviceLocator.Get<MemoryAllocator>();
			ModuleLoader::GetInstance().UnloadModules();
			ModuleLoader::Destroy();
			delete &allocator;
		}

		void EntryFunction()
		{
			std::cin.get();
			ProgramMain main;
			main.Initialize();
			main.Update();
			main.Shutdown();
		}
	}
}


#if VIPER_PLATFORM == VIPER_PLATFORM_WIN	// Windows
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	Viper::Platform::EntryFunction();
}

#elif VIPER_PLATFORM == VIPER_PLATFORM_LINUX  // Linux
int main()
{
	Viper::Platform::EntryFunction();
	return 0;
}

#elif VIPER_PLATFORM == VIPER_PLATFORM_MAC	// Mac
int main()
{
	Viper::Platform::EntryFunction();
	return 0;
}
#endif
