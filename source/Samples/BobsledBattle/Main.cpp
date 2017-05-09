#include "Pch.h"
#include <windows.h>
#include <strsafe.h>
#include "Core/ServiceLocator.h"
#include "Core/ModuleLoader.h"
#include "Memory/MemoryManager.h"
#include "Service/AudioManager.h"


#ifndef UNICODE
#define UNICODE
#endif


using namespace Viper;


void Initialize()
{
	MemoryManager* allocator = new MemoryManager();
	assert(allocator != nullptr);
	Core::ModuleLoader::CreateInstance(*allocator);
	Core::ModuleLoader::GetInstance().LoadModules("Config.ini");
}

void ShutDown()
{
	MemoryAllocator& allocator = ServiceLocator::GetInstance().GetMemoryAllocator();
	Core::ModuleLoader::GetInstance().UnloadModules();
	Core::ModuleLoader::Destroy();
	delete &allocator;
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	Initialize();

	WindowManager& windowManager = ServiceLocator::GetInstance().GetWindowManager();
	windowManager.Initialize();
	windowManager.CreateGameWindow(1280, 720, "Bobsled Battle");

	// Test Audio
	AudioManager& audioManager = ServiceLocator::GetInstance().GetAudioManager();
	audioManager.SetListener3dAttributes(Viper::Vector3(0, 0, 0), Viper::Vector3(0, 0, 1), Viper::Vector3(0, 1, 0));
	audioManager.LoadSoundBank("Content/Sounds/ZombieWars.bank");
	audioManager.LoadSoundBank("Content/Sounds/ZombieWars.strings.bank");
	audioManager.LoadSoundBankEvents("Content/Sounds/ZombieWars.bank");

	Viper::Vector3 position(0, 0, 0);
	audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
	audioManager.PlayEvent("event:/GattlingGun-Fire");

	while (windowManager.Update())
	{
		audioManager.Update();
		position.z += 0.5;
		audioManager.SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		Sleep(500);
	}

	windowManager.Shutdown();
	ShutDown();
	return 0;
}
