
#include "FmodAudioManager.h"
#include <cstdio>
#include <windows.h>
//#include <memory>
#include "MemoryManager.h"
//#include "DataArray.h"
#include "Types.h"

//using namespace Viper::Audio;

using namespace Viper;


int main()
{
	Audio::FmodAudioManager::CreateInstance();
	Audio::FmodAudioManager* manager = Audio::FmodAudioManager::GetInstance();
	manager->Init(10);

	manager->SetListener3dAttributes(Viper::Vector3(0, 0, 0), Viper::Vector3(0, 0, 1), Viper::Vector3(0, 1, 0));

	/*manager->LoadSound("sounds/gattling-gun.wav", true, true, false);
	manager->LoadSound("sounds/pistol.wav", true, true, false);
	manager->PlaySound("sounds/gattling-gun.wav");
	manager->PlaySound("sounds/pistol.wav");*/
	manager->LoadBank("sounds/ZombieWars.bank");
	manager->LoadBank("sounds/ZombieWars.strings.bank");
	manager->LoadEvent("event:/GattlingGun-Fire");

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
