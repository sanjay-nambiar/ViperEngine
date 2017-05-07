#include "Core/ModuleExports.h"
#include "FmodAudioManager.h"
#include "Core/MemoryManager.h"

Viper::AudioManager* InitializeAudio(uint32_t maxChannels, Viper::MemoryManager& memoryManager)
{
	return new Viper::Audio::FmodAudioManager(maxChannels, memoryManager);
}
