#include "Core/ServiceLocator.h"
#include "FmodAudioManager.h"
#include "Platform/ModuleExports.h"

using namespace Viper;
using namespace Memory;
using namespace Audio;

#define CHANNELS_CONFIG_ATTRIBUTE "Channels"

void InitializeModule(ServiceLocator& serviceLocator, const std::unordered_map<std::string, std::string>& configAttributes)
{
	std::uint32_t maxChannels = std::atoi(configAttributes.at(CHANNELS_CONFIG_ATTRIBUTE).c_str());
	MemoryAllocator& allocator = serviceLocator.GetMemoryAllocator();
	void* memBlock = allocator.Allocate(sizeof(Audio::FmodAudioManager), 1);
	assert(memBlock != nullptr);
	AudioManager* audioManager = new(memBlock) Audio::FmodAudioManager(maxChannels, serviceLocator.GetMemoryAllocator());
	serviceLocator.Provide(*audioManager);
}
