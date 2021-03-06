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
	auto& allocator = serviceLocator.Get<MemoryAllocator>();
	auto memBlock = allocator.Allocate<Audio::FmodAudioManager>(1);
	assert(memBlock != nullptr);
	AudioManager* audioManager = new(memBlock) Audio::FmodAudioManager(maxChannels, serviceLocator.Get<MemoryAllocator>());
	serviceLocator.Provide(*audioManager);
}
