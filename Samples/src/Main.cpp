
#include <cstdio>
#include <windows.h>
#include "FmodAudioManager.h"
#include "MemoryManager.h"
#include "service/AudioManager.h"


using namespace Viper;


int main()
{
	MemoryManager allocator;
	AudioManager* manager = new Audio::FmodAudioManager(100, allocator);

	manager->SetListener3dAttributes(Viper::Vector3(0, 0, 0), Viper::Vector3(0, 0, 1), Viper::Vector3(0, 1, 0));
	manager->LoadSoundBank("sounds/ZombieWars.bank");
	manager->LoadSoundBank("sounds/ZombieWars.strings.bank");
	manager->LoadSoundBankEvents("sounds/ZombieWars.bank");

	Viper::Vector3 position(0, 0, 0);
	manager->SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
	manager->PlayEvent("event:/GattlingGun-Fire");	

	do
	{
		manager->Update();
		position.z += 0.5;
		manager->SetEvent3dAttributes("event:/GattlingGun-Fire", position, Viper::Vector3(0, 0, 0));
		Sleep(500);
	}
	while (true);

	std::printf("Press any key to continue...");
	std::getchar();
}
