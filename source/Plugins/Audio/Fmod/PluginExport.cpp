#include "Core/ModuleExports.h"
#include "Core/ServiceLocator.h"
#include "FmodAudioManager.h"

using namespace Viper;

void ProvideAudio(uint32_t maxChannels, ServiceLocator& serviceLocator)
{
	MemoryAllocator& allocator = serviceLocator.GetMemoryAllocator();
	void* memBlock = allocator.Allocate(sizeof(Audio::FmodAudioManager), 1);
	assert(memBlock != nullptr);
	AudioManager* audioManager = new(memBlock) Audio::FmodAudioManager(maxChannels, serviceLocator.GetMemoryAllocator());
	serviceLocator.Provide(*audioManager);
}
